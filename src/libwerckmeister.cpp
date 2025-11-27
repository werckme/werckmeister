#include "libwerckmeister.h"
#include <boost/di.hpp>
#include <boost/di/extension/scopes/scoped.hpp>
#include <iostream>
#include <memory>
#include "SheetLibProgram.h"
#include <parser/parser.h>
#include <com/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <com/ConsoleLogger.h>
#include <com/FileLogger.h>
#include <compiler/LoggerAndWarningsCollector.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <compiler/EventLogger.h>
#include <documentModel/Document.h>
#include <compiler/DefinitionsServer.h>
#include <com/midi.hpp>
#include <app/DummyFileWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <app/TimelineVisitor.hpp>
#include <compiler/SheetNavigator.h>
#include <conductor/ConductionsPerformer.h>
#include "FactoryConfig.h"
#include <compiler/CompoundVisitor.hpp>
#include <compiler/EventInformationServer.h>
#include <app/MidiFileWriter.h>
#include <app/FluidSynthWriter.h>
#include <com/config.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <list>

#define wmassert(exp, msg) if(!(exp)) LOG(msg)
#define STRVERSION SHEET_VERSION " lib 0.13"

namespace
{
	com::ILoggerPtr createLogger()
	{
		const char* logPath = std::getenv("WMLIB_LOGFILE_PATH");
		com::ILoggerPtr logger;
		if (logPath)
		{
			logger = std::make_shared<com::FileLogger>(logPath);
		} else 
		{
			logger = std::make_shared<com::ConsoleLogger>();
		}
		logger->logLevel(com::FileLogger::LevelDebug);
		return logger;
	}
	typedef SheetLibProgram CompilerProgram;
	struct CueDate
	{
		com::String cueText;
		unsigned int timePointMillis = 0;
	};
	typedef std::vector<CueDate> CueDates;
	struct Session 
	{
		std::mutex processMidiMutex;
		std::list<com::midi::Event> tmpMidiEvents;
		std::vector<unsigned char> tmpMidiEventBff; // single events
		std::vector<unsigned char> tmpMidiDataBff; // midi file
		com::ILoggerPtr logger = createLogger();
		com::midi::MidiPtr midiFile;
		app::FluidSynthWriterPtr fluidSynth;
		CueDates cueDates;
	};
}

#define LOG(x) _logger->babble(WMLogLambda(log << x))
#define ERR(x) _logger->error(WMLogLambda(log << x))


static com::midi::MidiPtr createMidiFile(Session* session, int argc, const char** argv);
static void handleIfMetaEvent(Session*, const com::midi::Event&);
static void eatTmpEvents(Session* session);
static void eatTmpMidiData(Session *session);

extern "C"  
{
	WERCKM_EXPORT const char * wm_getStrVersion()
	{
		return STRVERSION;
	}

	WERCKM_EXPORT WmSession wm_createSession()
	{
		auto session = new Session();
		session->tmpMidiEventBff.reserve(255);
		return session;
	}

  	WERCKM_EXPORT int wm_releaseSession(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		LOG("wm_releaseSession");
		if (session->fluidSynth)
		{
			session->fluidSynth->tearDownSynth();
		}
		session->fluidSynth.reset();
		session->midiFile.reset();
		session->logger.reset();
		delete session;
		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_compile(WmSession sessionPtr, const char * sourcePath)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		LOG("wm_compile" << ": " << sourcePath);
		const char * args[] = {"noProgramName", sourcePath, "--verbose"};
		const int numArgs = sizeof(args)/sizeof(args[0]);
		try 
		{
			session->midiFile = createMidiFile(session, numArgs, &args[0]);
			wmassert(session->midiFile, "expected session->midi");
			LOG("session created " << session->midiFile->byteSize() << " bytes");
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to create compiler program: " << ex.what());
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to create compiler program");
			return WERCKM_ERR;
		}
	}

	WERCKM_EXPORT bool wm_iscompiled(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return false;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		return session->midiFile.get() != nullptr;
	}

	WERCKM_EXPORT int wm_initSynth(WmSession sessionPtr, const char *libPath, int sampleRate)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try
		{
			std::lock_guard<std::mutex> guard(session->processMidiMutex);
			LOG("wm_initSynth: " << libPath << ", sample rate:" << sampleRate);
			session->fluidSynth = std::make_shared<app::FluidSynthWriter>(_logger);
			session->fluidSynth->libPath(libPath);
			session->fluidSynth->sampleRate((double)sampleRate);
			session->fluidSynth->initSynth("");
			if (session->tmpMidiEvents.empty() == false)
			{
				eatTmpEvents(session);
			}
			if (session->tmpMidiDataBff.empty() == false)
			{
				eatTmpMidiData(session);
			}
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to init synth: " << ex.what());
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to init synth");
			return WERCKM_ERR;
		}
		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_copyCompiledMidiDataToSynth(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		LOG("wm_copyCompiledMidiDataToSynth");
		if (session->fluidSynth.get() == nullptr)
		{
			return WERCKM_OK;
		}
		if (session->midiFile.get() == nullptr)
		{
			ERR("no midi file");
			return WERCKM_ERR;
		}
		LOG("copy " << session->midiFile->ctracks().size() <<  " tracks");
		try 
		{
			for (const auto& track : session->midiFile->ctracks())
			{
				LOG("copy " << track->events().numEvents() << " events");
				for (const auto& event : track->events())
				{
					session->fluidSynth->addEvent(event);
					handleIfMetaEvent(session, event);
				}
			}
		}
		catch (const std::exception& ex)
		{
			ERR("failed copy midi data: " << ex.what());
				return WERCKM_ERR;
		}
		catch (...)
		{
			ERR("failed to copy midi data");
				return WERCKM_ERR;
		}

		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_setSoundFontHome(WmSession, const char *sfHomePath)
	{
		com::getWerckmeister().addSearchPath(sfHomePath);
		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_synthRender(WmSession sessionPtr, int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try
		{
			if (session->fluidSynth.get() == nullptr)
			{
				return WERCKM_OK;
			}
			session->fluidSynth->render(len, lout, loff, lincr, rout, roff, rincr);
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to render: " << ex.what());
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to render");
			return WERCKM_ERR;
		}
	}

	WERCKM_EXPORT double wm_getSongPositionSeconds(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		if (!session->fluidSynth)
		{
			return 0;
		}
		return session->fluidSynth->getSongPositionSeconds();
	}

	WERCKM_EXPORT int wm_getNumMidiCuePoints(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			
			return (int)session->cueDates.size();
		} 
		catch(...)
		{
			ERR("failed to get cue dates size");
			return WERCKM_ERR;
		}
	}

	WERCKM_EXPORT const char * wm_getMidiCuePointText(WmSession sessionPtr, int index)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			const auto& cueDate = session->cueDates.at(index);
			return cueDate.cueText.c_str();
		}
		catch(...)
		{
			ERR("failed to get cue text");
			return 0;
		}
	}

	WERCKM_EXPORT unsigned int wm_getMidiCuePositionMillis(WmSession sessionPtr, int index)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			const auto& cueDate = session->cueDates.at(index);
			return cueDate.timePointMillis;
		}
		catch(...)
		{
			ERR("failed to get cue position");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_writeToFile(WmSession sessionPtr, const char* outputPath)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		if (!session->fluidSynth)
		{
			return 0;
		}
		auto _logger = session->logger;
		try 
		{
			auto bpm = session->fluidSynth->tempo();
			auto totalLength = session->midiFile->duration() * 60.0 / (bpm * com::PPQ);
			session->fluidSynth->renderToFile(outputPath, totalLength);
			return 0;
		}
		catch(...)
		{
			ERR("failed to get cue position");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_addMidiEvent(WmSession sessionPtr, double tickPos, const unsigned char* data, unsigned int length)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			std::lock_guard<std::mutex> guard(session->processMidiMutex);
			bool synthIsReady = session->fluidSynth.get() != nullptr;
			com::midi::Event event;

			session->tmpMidiEventBff.clear();
			session->tmpMidiEventBff.push_back(0);

			for(int i = 0; i<length; ++i)
			{
				session->tmpMidiEventBff.push_back(data[i]);
			}

			event.read(0, session->tmpMidiEventBff.data(), length + 1);
			event.absPosition(tickPos);

			if (synthIsReady)
			{
				session->fluidSynth->addEvent(event);
				handleIfMetaEvent(session, event);
			} else
			{
				session->tmpMidiEvents.push_back(event);
			}
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to addMidiEvent: " << length << " " << ex.what());
			return WERCKM_ERR;
		}
		catch(...)
		{
			ERR("failed to addMidiEvent");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_addMidiFileData(WmSession sessionPtr, const unsigned char* data, unsigned int length)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			std::lock_guard<std::mutex> guard(session->processMidiMutex);
			bool synthIsReady = session->fluidSynth.get() != nullptr;

			if (synthIsReady)
			{
				session->fluidSynth->setMidiFileData(data, length);
			} else
			{
				session->tmpMidiDataBff = std::vector<unsigned char>(data, data + length);
			}
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to addMidiEvent: " << length << " " << ex.what());
			return WERCKM_ERR;
		}
		catch(...)
		{
			ERR("failed to addMidiEvent");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_setCC(WmSession sessionPtr, int ch, int cc, int value)
	{
		if (sessionPtr == nullptr)
		{
			return 0;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		if (!session->fluidSynth)
		{
			return 0;
		}
		auto _logger = session->logger;
		try 
		{
			session->fluidSynth->setCC(ch, cc, value);
			return 0;
		}
		catch(...)
		{
			ERR("failed to set cc");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_setJumpPoints(WmSession sessionPtr, WmJumpPoint* jumpPoints, int length)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			app::FluidSynthWriter::JumpPoints jmps;
			for(int i=0; i<length; ++i)
			{
				app::FluidSynthWriter::JumpPoint jmp;
				jmp.fromPositionTicks = jumpPoints[i].fromPositionSeconds * com::PPQ;
				jmp.toPositionTicks = jumpPoints[i].toPositionSeconds * com::PPQ;
			}
			session->fluidSynth->setJumpPoints(std::move(jmps));
			return WERCKM_ERR;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to set jump points: " << ex.what());
			return WERCKM_ERR;
		}
		catch(...)
		{
			ERR("failed to set jump points");
			return 0;
		}
	}

	WERCKM_EXPORT int wm_setActiveJumpPoint(WmSession sessionPtr, int jumpPointIndex)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		try 
		{
			session->fluidSynth->setActiveJumpPoint(jumpPointIndex);
			return WERCKM_ERR;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to setActiveJumpPoint: " << jumpPointIndex << " " << ex.what());
			return WERCKM_ERR;
		}
		catch(...)
		{
			ERR("failed to setActiveJumpPoint");
			return 0;
		}
	}
}

static void eatTmpEvents(Session* session)
{
	auto _logger = session->logger;
	LOG("process " << session->tmpMidiEvents.size() << " events");
	for(const auto& event : session->tmpMidiEvents)
	{
		try 
		{
			session->fluidSynth->addEvent(event);
			handleIfMetaEvent(session, event);
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to addMidiEvent:" << ex.what());
		}
		catch(...)
		{
			ERR("failed to addMidiEvent");
		}
	}
	session->tmpMidiEvents.clear();
}

static void eatTmpMidiData(Session* session)
{
	auto _logger = session->logger;
	LOG("process " << session->tmpMidiDataBff.size() << " bytes of midi data");
	try 
	{
		session->fluidSynth->setMidiFileData(session->tmpMidiDataBff.data(), session->tmpMidiDataBff.size());
	}
	catch (const std::exception &ex) 
	{
		ERR("failed to addMidiEvent:" << ex.what());
	}
	catch(...)
	{
		ERR("failed to addMidiEvent");
	}
	session->tmpMidiDataBff.clear();
}

static com::midi::MidiPtr createMidiFile(Session* session, int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = compiler;
	namespace co = conductor;
	namespace pr = parser;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
	programOptionsPtr->parseProgrammArgs(argc, argv);

	auto documentPtr = std::make_shared<documentModel::Document>();
	auto midiFile = com::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isJsonModeSet();
	auto _logger = session->logger;
	auto injector = di::make_injector(
		di::bind<pr::IDocumentParser>().to<pr::DocumentParser>().in(di::extension::scoped),
		di::bind<cp::ICompiler>().to<cp::Compiler>().in(di::extension::scoped),
		di::bind<cp::ISheetTemplateRenderer>().to<cp::SheetTemplateRenderer>().in(di::extension::scoped),
		di::bind<cp::ASheetEventRenderer>().to<cp::SheetEventRenderer>().in(di::extension::scoped),
		di::bind<cp::IContext>().to<cp::MidiContext>().in(di::extension::scoped),
		di::bind<cp::IPreprocessor>().to<cp::Preprocessor>().in(di::extension::scoped),
		di::bind<cp::ISheetNavigator>().to<cp::SheetNavigator>().in(di::extension::scoped),
		di::bind<co::IConductionsPerformer>().to<co::ConductionsPerformer>().in(di::extension::scoped),
		di::bind<cp::IEventInformationServer>().to<cp::EventInformationServer>().in(di::extension::scoped),
		di::bind<ICompilerProgramOptions>().to(programOptionsPtr),
		di::bind<documentModel::Document>().to(documentPtr),
		di::bind<compiler::IDefinitionsServer>().to<compiler::DefinitionsServer>().in(di::extension::scoped),
		di::bind<com::midi::Midi>().to(midiFile),
		di::bind<app::IDocumentWriter>().to([&](const auto& injector) -> app::IDocumentWriterPtr
			{
				return injector.template create<std::shared_ptr<app::DummyFileWriter>>();
			}),
		di::bind<cp::ICompilerVisitor>().to([&](const auto& injector) -> cp::ICompilerVisitorPtr
			{
				if (needTimeline)
				{
					return injector.template create<std::shared_ptr< cp::CompoundVisitor_N2<app::DefaultTimeline, cp::EventInformationServer> >>();
				}
				return injector.template create<std::shared_ptr<cp::EventInformationServer>>();
			}),
		di::bind<com::ILogger>().to([&](const auto& injector) -> com::ILoggerPtr
			{
				// return injector.template create<std::shared_ptr<com::ConsoleLogger>>();
				return _logger;
			}));

	FactoryConfig factory(injector);
	factory.init();
	auto program = injector.create<CompilerProgram>();
	program.prepareEnvironment();
	program.execute();
	return midiFile;
}

static void handleIfMetaEvent(Session* session, const com::midi::Event& event)
{
	auto _logger = session->logger;
	if (event.eventType() != com::midi::MetaEvent)
	{
		return;
	}
	if (event.metaEventType() == com::midi::CuePoint)
	{
		auto& cueDates = session->cueDates;
		auto bpm = session->fluidSynth->tempo();
		CueDate cue;
		cue.cueText = com::midi::Event::MetaGetStringValue(event.metaData(), event.metaDataSize());
		cue.timePointMillis = 60.0 * 1000.0 * event.absPosition() / (bpm * com::PPQ);
		cueDates.push_back(cue);
	}
}