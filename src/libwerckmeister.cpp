#include "libwerckmeister.h"
#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetLibProgram.h"
#include <parser/parser.h>
#include <com/werckmeister.hpp>
#include <CompilerProgramOptions.h>
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
#include <com/config.hpp>

#define LOG(x) std::cout << "[WM_LOG] " << x << std::endl;
#define assert(exp, msg) if(!(exp)) LOG(msg)

namespace
{
	typedef std::shared_ptr<SheetLibProgram> CompilerProgramPtr;
	struct Session 
	{
		CompilerProgramPtr compilerProgramPtr;
		com::midi::MidiPtr midi() const { return compilerProgramPtr->getMidiFile(); }
	};
}


extern "C"  
{
	static CompilerProgramPtr createCompilerProgram(int argc, const char **argv);

	WM_EXPORT const char * wm_getStrVersion()
	{
		static std::string version = std::string(SHEET_VERSION) + " lib 0.11";
		return version.c_str();
	}

	WM_EXPORT WmSession wm_createSession(const char * sourcePath)
	{
		LOG("wm_createSession" << ": " << sourcePath)
		const char * args[] = {sourcePath, "--verbose"};
		const int numArgs = sizeof(args)/sizeof(args[0]);
		auto session = new Session();
		try 
		{
			session->compilerProgramPtr = createCompilerProgram(numArgs, &args[0]);
			assert(session->compilerProgramPtr, "expected compilerProgramPtr");
			//assert(session->midi(), "expected session->midi");
			//LOG("session created " << session->midi()->tracks().size())
			//LOG("session created " << session->midi()->byteSize() << " bytes")
			return session;
		}
		catch (...) 
		{
			std::cout << "failed to create compiler program" << std::endl;
			return nullptr;
		}
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


	static CompilerProgramPtr createCompilerProgram(int argc, const char **argv)
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
		bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
		auto injector = di::make_injector(
			di::bind<pr::IDocumentParser>().to<pr::DocumentParser>().in(di::singleton), 
			di::bind<cp::ICompiler>().to<cp::Compiler>().in(di::singleton), 
			di::bind<cp::ISheetTemplateRenderer>().to<cp::SheetTemplateRenderer>().in(di::singleton), 
			di::bind<cp::ASheetEventRenderer>().to<cp::SheetEventRenderer>().in(di::singleton), 
			di::bind<cp::IContext>().to<cp::MidiContext>().in(di::singleton),
			di::bind<cp::IPreprocessor>().to<cp::Preprocessor>().in(di::singleton), 
			di::bind<cp::ISheetNavigator>().to<cp::SheetNavigator>().in(di::singleton), 
			di::bind<co::IConductionsPerformer>().to<co::ConductionsPerformer>().in(di::singleton), 
			di::bind<cp::IEventInformationServer>().to<cp::EventInformationServer>().in(di::singleton),
			di::bind<ICompilerProgramOptions>().to(programOptionsPtr), 
			di::bind<documentModel::Document>().to(documentPtr), 
			di::bind<compiler::IDefinitionsServer>().to<compiler::DefinitionsServer>().in(di::singleton), 
			di::bind<com::midi::Midi>().to(midiFile), 
			di::bind<app::IDocumentWriter>().to([&](const auto &injector) -> app::IDocumentWriterPtr
			{
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
				return injector.template create<std::shared_ptr<com::FileLogger>>();
			}));
		FactoryConfig factory(injector);
		factory.init();
		auto program = injector.create<CompilerProgramPtr>();
		program->prepareEnvironment();
		program->execute();
		return program;
	}
}