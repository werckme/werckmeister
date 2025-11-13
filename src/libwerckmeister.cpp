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

#define assert(exp, msg) if(!(exp)) LOG(msg)
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
	struct Session 
	{
		com::ILoggerPtr logger = createLogger();
		com::midi::MidiPtr midiFile;
		app::FluidSynthWriterPtr fluidSynth;
	};
}

#define LOG(x) _logger->babble(WMLogLambda(log << x));
#define ERR(x) _logger->error(WMLogLambda(log << x));


static com::midi::MidiPtr createMidiFile(Session* session, int argc, const char** argv);

extern "C"  
{
	WERCKM_EXPORT const char * wm_getStrVersion()
	{
		return STRVERSION;
	}

	WERCKM_EXPORT WmSession wm_createSession()
	{
		auto session = new Session();
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
		LOG("wm_releaseSession")
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
		LOG("wm_compile" << ": " << sourcePath)
		const char * args[] = {"noProgramName", sourcePath, "--verbose"};
		const int numArgs = sizeof(args)/sizeof(args[0]);
		try 
		{
			session->midiFile = createMidiFile(session, numArgs, &args[0]);
			assert(session->midiFile, "expected session->midi");
			LOG("session created " << session->midiFile->byteSize() << " bytes")
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to create compiler program: " << ex.what())
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to create compiler program")
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
			LOG("wm_initSynth: " << libPath << ", sample rate:" << sampleRate)
			session->fluidSynth = std::make_shared<app::FluidSynthWriter>(_logger);
			session->fluidSynth->libPath(libPath);
			session->fluidSynth->sampleRate((double)sampleRate);
			session->fluidSynth->initSynth("");
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to init synth: " << ex.what())
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to init synth")
			return WERCKM_ERR;
		}
		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_copyMidiDataToSynth(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return WERCKM_ERR;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		auto _logger = session->logger;
		LOG("wm_copyMidiDataToSynth")
		if (session->fluidSynth.get() == nullptr)
		{
			return WERCKM_OK;
		}
		if (session->midiFile.get() == nullptr)
		{
			ERR("no midi file")
			return WERCKM_ERR;
		}
		LOG("copy " << session->midiFile->ctracks().size() <<  " tracks")
		try 
		{
			for (const auto& track : session->midiFile->ctracks())
			{
				LOG("copy " << track->events().numEvents() << " events")
					for (const auto& event : track->events())
					{
						if (!session->fluidSynth->addEvent(event))
						{
							LOG("copy event " << event.toString() << " failed")
								continue;
						}
					}
			}
		}
		catch (const std::exception& ex)
		{
			ERR("failed copy midi data: " << ex.what())
				return WERCKM_ERR;
		}
		catch (...)
		{
			ERR("failed to copy midi data")
				return WERCKM_ERR;
		}

		return WERCKM_OK;
	}

	WERCKM_EXPORT int wm_setSoundFontHome(WmSession, const char *sfHomePath)
	{
		com::getWerckmeister().addSearchPath(sfHomePath);
		return WERCKM_OK;
	}

	int wm_synthRender(WmSession sessionPtr, int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr)
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
			if(!wm_iscompiled(sessionPtr))
			{
				return WERCKM_OK;
			}
			session->fluidSynth->render(len, lout, loff, lincr, rout, roff, rincr);
			return WERCKM_OK;
		}
		catch (const std::exception &ex) 
		{
			ERR("failed to render: " << ex.what())
			return WERCKM_ERR;
		}
		catch (...) 
		{
			ERR("failed to render")
			return WERCKM_ERR;
		}
	}
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