
#include <boost/di.hpp>
#include <boost/di/extension/scopes/scoped.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "SheetPlayerProgram.h"
#include <parser/parser.h>
#include <com/werckmeister.hpp>
#include <PlayerProgramOptions.h>
#include <com/ConsoleLogger.h>
#include <compiler/LoggerAndWarningsCollector.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <compiler/EventLogger.h>
#include <sheet/Document.h>
#include <com/DefinitionsServer.h>
#include <com/midi.hpp>
#include <app/MidiPlayer.h>
#include <app/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <app/TimelineVisitor.hpp>
#include <app/SheetWatcher.h>
#include <app/PlayerTimePrinter.h>
#include <app/DiContainerWrapper.h>
#include <app/ISheetWatcherHandler.h>
#include <app/Funkfeuer.h>
#include <app/UdpSender.hpp>
#include <app/NullStringSender.hpp>
#include <compiler/SheetNavigator.h>
#include <conductor/ConductionsPerformer.h>
#include "FactoryConfig.h"

#ifdef SHEET_USE_BOOST_TIMER
#include "app/boostTimer.h"
#else
#include "app/os.hpp"
#endif

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

typedef sheet::compiler::EventLogger<com::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<com::ConsoleLogger> WarningsCollectorWithConsoleLogger;

int startPlayer(std::shared_ptr<PlayerProgramOptions> programOptionsPtr);

funk::UdpSenderPtr _udpSender = nullptr;

int main(int argc, const char** argv)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	auto programOptionsPtr = std::make_shared<PlayerProgramOptions>();
	try {
		programOptionsPtr->parseProgrammArgs(argc, argv);
		if (programOptionsPtr->isUdpSet()) {
			auto host = programOptionsPtr->getUdpHostname();
			_udpSender = std::make_shared<funk::UdpSender>(host);
			_udpSender->start();
		}
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	int returnCode = 0;
	
#ifdef SHEET_USE_BOOST_TIMER
       std::thread boost_asio_([] {
           app::BoostTimer::io_run();
       });
#endif

	do {
		returnCode = startPlayer(programOptionsPtr);
	} while (returnCode == SheetPlayerProgram::RetCodeRestart);
	

#ifdef SHEET_USE_BOOST_TIMER
	app::BoostTimer::io_stop();
	boost_asio_.join();
#endif
	if (_udpSender) {
		_udpSender->stop();
	}
	return returnCode;
}

int startPlayer(std::shared_ptr<PlayerProgramOptions> programOptionsPtr)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	namespace co = sheet::conductor;
	app::SheetWatcherHandlersPtr sheetWatcherHandlers = std::make_shared<app::SheetWatcherHandlers>();
	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = com::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isUdpSet();
	app::DiContainerWrapper<app::IPlayerLoopVisitorPtr> loopVisitors;
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()									 .to<cp::DocumentParser>()			.in(di::extension::scoped)
		, di::bind<cp::ICompiler>()											 .to<cp::Compiler>()				.in(di::extension::scoped)
		, di::bind<cp::ISheetTemplateRenderer>()							 .to<cp::SheetTemplateRenderer>()	.in(di::extension::scoped)
		, di::bind<cp::ASheetEventRenderer>()								 .to<cp::SheetEventRenderer>()		.in(di::extension::scoped)
		, di::bind<cp::IContext>()											 .to<cp::MidiContext>()				.in(di::extension::scoped)
		, di::bind<cp::IPreprocessor>()										 .to<cp::Preprocessor>()			.in(di::extension::scoped)
		, di::bind<cp::ISheetNavigator>()									 .to<cp::SheetNavigator>()			.in(di::extension::scoped)
		, di::bind<co::IConductionsPerformer>()								 .to<co::ConductionsPerformer>()	.in(di::extension::scoped)
		, di::bind<ICompilerProgramOptions>()								 .to(programOptionsPtr)
		, di::bind<sheet::Document>()										 .to(documentPtr)
		, di::bind<com::IDefinitionsServer>()								 .to<com::DefinitionsServer>()		.in(di::extension::scoped)
		, di::bind<com::midi::Midi>()										 .to(midiFile)
		, di::bind<app::SheetWatcherHandlers>()							 .to(sheetWatcherHandlers)
		, di::bind<app::DiContainerWrapper<app::IPlayerLoopVisitorPtr>>().to(loopVisitors)
		, di::bind<app::IDocumentWriter>()		.to([&](const auto &injector) -> app::IDocumentWriterPtr
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::unique_ptr<app::JsonWriter>>();
			}
			return injector.template create<std::unique_ptr<app::MidiPlayer>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<app::DefaultTimeline>>();
			}
			return injector.template create< std::unique_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<com::ILogger>()					.to([&](const auto &injector) -> com::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
		, di::bind<app::IStringSender>()					.to([&](const auto &injector) -> app::IStringSenderPtr 
		{
			if (programOptionsPtr->isUdpSet()) {
				return _udpSender;
			}
			return injector.template create<std::shared_ptr<app::NullStringSender>>();
		})		
	);
	try {
		if (!programOptionsPtr->isNoTimePrintSet()) {
			// unique_ptr to shared_ptr is for purpose: create shared_ptr means singleton, but in this case we need unique
			std::shared_ptr<app::PlayerTimePrinter> vis = injector.create<std::unique_ptr<app::PlayerTimePrinter>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isWatchSet()) {
			std::shared_ptr<app::SheetWatcher> vis = injector.create<std::unique_ptr<app::SheetWatcher>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isUdpSet()) {
			std::shared_ptr<app::Funkfeuer> vis = injector.create<std::unique_ptr<app::Funkfeuer>>();
			loopVisitors.container.push_back(vis);
		}
		sheet::FactoryConfig factory(injector);
		factory.init();
		auto program = injector.create<SheetPlayerProgram*>();
		sheetWatcherHandlers->container.push_back(program);
		program->prepareEnvironment();
		auto result = program->execute();
		sheetWatcherHandlers->container.clear();
		delete program;
		return result;
	} catch (const com::Exception &ex) {
		std::cerr << ex.toString() << std::endl;
		return 1;
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	
}