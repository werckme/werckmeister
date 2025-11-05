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
#include <com/config.hpp>

#define LOG(x) std::cout << "[WM_LOG] " << x << std::endl;
#define assert(exp, msg) if(!(exp)) LOG(msg)

#define STRVERSION SHEET_VERSION " lib 0.13"

namespace
{
	typedef SheetLibProgram CompilerProgram;
	struct Session 
	{
		com::midi::MidiPtr midiFile;
	};
}


extern "C"  
{
	static com::midi::MidiPtr createMidiFile(int argc, const char **argv);

	WM_EXPORT const char * wm_getStrVersion()
	{
		return STRVERSION;
	}

	WM_EXPORT WmSession wm_createSession()
	{
		LOG("wm_createSession")
		auto session = new Session();
		return session;
	}

  	WM_EXPORT int wm_releaseSession(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return -1;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		delete session;
		return 0;
	}

	WM_EXPORT int wm_compile(WmSession sessionPtr, const char * sourcePath)
	{
		if (sessionPtr == nullptr)
		{
			return -1;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		LOG("wm_compile" << ": " << sourcePath)
		const char * args[] = {"noProgramName", sourcePath, "--verbose"};
		const int numArgs = sizeof(args)/sizeof(args[0]);
		try 
		{
			session->midiFile = createMidiFile(numArgs, &args[0]);
			assert(session->midiFile, "expected session->midi");
			LOG("session created " << session->midiFile->byteSize() << " bytes")
			return 0;
		}
		catch (const std::exception &ex) 
		{
			std::cout << "failed to create compiler program: " << ex.what() << std::endl;
			return -1;
		}
		catch (...) 
		{
			std::cout << "failed to create compiler program: unknown reason" << std::endl;
			return -1;
		}
	}

	WM_EXPORT bool wm_iscompiled(WmSession sessionPtr)
	{
		if (sessionPtr == nullptr)
		{
			return false;
		}
		Session* session = reinterpret_cast<Session*>(sessionPtr);
		return session->midiFile.get() != nullptr;
	}

	static com::midi::MidiPtr createMidiFile(int argc, const char **argv)
	{
		namespace di = boost::di;
		namespace cp = compiler;
		namespace co = conductor;
		namespace pr = parser;
		auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
		programOptionsPtr->parseProgrammArgs(argc, argv);
		programOptionsPtr->logFilePath = "/home/samba/wm-log.txt"; // TODO: JUST FOR DEBUG, REMOVE

		auto documentPtr = std::make_shared<documentModel::Document>();
		auto midiFile = com::getWerckmeister().createMidi();
		bool needTimeline = programOptionsPtr->isJsonModeSet();
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
			di::bind<app::IDocumentWriter>().to([&](const auto &injector) -> app::IDocumentWriterPtr
			{
				//return injector.template create<std::shared_ptr<app::DummyFileWriter>>();
				return injector.template create<std::shared_ptr<app::DummyFileWriter>>();
			}),
			di::bind<cp::ICompilerVisitor>().to([&](const auto &injector) -> cp::ICompilerVisitorPtr
			{
				if (needTimeline)
				{
					return injector.template create<std::shared_ptr< cp::CompoundVisitor_N2<app::DefaultTimeline, cp::EventInformationServer> >>();
				}
				return injector.template create<std::shared_ptr<cp::EventInformationServer>>();
			}),
			di::bind<com::ILogger>().to([&](const auto &injector) -> com::ILoggerPtr
			{
				return injector.template create<std::shared_ptr<com::ConsoleLogger>>();
			}));

		FactoryConfig factory(injector);
		factory.init();
		auto program = injector.create<CompilerProgram>();
		program.prepareEnvironment();
		program.execute();
		return midiFile;
	}
}