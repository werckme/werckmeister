
#include <boost/di.hpp>
#include <boost/di/extension/scopes/scoped.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "SheetPlayerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <PlayerProgramOptions.h>
#include <fm/ConsoleLogger.h>
#include <compiler/LoggerAndWarningsCollector.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <compiler/EventLogger.h>
#include <sheet/Document.h>
#include <fm/DefinitionsServer.h>
#include <fm/midi.hpp>
#include <fmapp/MidiPlayer.h>
#include <fmapp/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <fmapp/TimelineVisitor.hpp>
#include <fmapp/SheetWatcher.h>
#include <fmapp/PlayerTimePrinter.h>
#include <fmapp/DiContainerWrapper.h>
#include <fmapp/ISheetWatcherHandler.h>
#include <fmapp/Funkfeuer.h>
#include <fmapp/UdpSender.hpp>
#include <fmapp/NullStringSender.hpp>
#include <compiler/SheetNavigator.h>

#ifdef SHEET_USE_BOOST_TIMER
#include "fmapp/boostTimer.h"
#else
#include "fmapp/os.hpp"
#endif


typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;

int startPlayer(std::shared_ptr<PlayerProgramOptions> programOptionsPtr);

funk::UdpSenderPtr _udpSender = nullptr;

int main(int argc, const char** argv)
{
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
           fmapp::BoostTimer::io_run();
       });
#endif

	do {
		returnCode = startPlayer(programOptionsPtr);
	} while (returnCode == SheetPlayerProgram::RetCodeRestart);
	

#ifdef SHEET_USE_BOOST_TIMER
	fmapp::BoostTimer::io_stop();
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
	fmapp::SheetWatcherHandlersPtr sheetWatcherHandlers = std::make_shared<fmapp::SheetWatcherHandlers>();
	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isUdpSet();
	fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr> loopVisitors;
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()									 .to<cp::DocumentParser>()			.in(di::extension::scoped)
		, di::bind<cp::ICompiler>()											 .to<cp::Compiler>()				.in(di::extension::scoped)
		, di::bind<cp::ISheetTemplateRenderer>()							 .to<cp::SheetTemplateRenderer>()	.in(di::extension::scoped)
		, di::bind<cp::ASheetEventRenderer>()								 .to<cp::SheetEventRenderer>()		.in(di::extension::scoped)
		, di::bind<cp::IContext>()											 .to<cp::MidiContext>()				.in(di::extension::scoped)
		, di::bind<cp::IPreprocessor>()										 .to<cp::Preprocessor>()			.in(di::extension::scoped)
		, di::bind<cp::ISheetNavigator>()									 .to<cp::SheetNavigator>()			.in(di::extension::scoped)
		, di::bind<ICompilerProgramOptions>()								 .to(programOptionsPtr)
		, di::bind<sheet::Document>()										 .to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()								 .to<fm::DefinitionsServer>()		.in(di::extension::scoped)
		, di::bind<fm::midi::Midi>()										 .to(midiFile)
		, di::bind<fmapp::SheetWatcherHandlers>()							 .to(sheetWatcherHandlers)
		, di::bind<fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr>>().to(loopVisitors)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::unique_ptr<fmapp::JsonWriter>>();
			}
			return injector.template create<std::unique_ptr<fmapp::MidiPlayer>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<fmapp::DefaultTimeline>>();
			}
			return injector.template create< std::unique_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<fm::ILogger>()					.to([&](const auto &injector) -> fm::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
		, di::bind<fmapp::IStringSender>()					.to([&](const auto &injector) -> fmapp::IStringSenderPtr 
		{
			if (programOptionsPtr->isUdpSet()) {
				return _udpSender;
			}
			return injector.template create<std::shared_ptr<fmapp::NullStringSender>>();
		})		
	);
	try {
		if (!programOptionsPtr->isNoTimePrintSet()) {
			// unique_ptr to shared_ptr is for purpose: create shared_ptr means singleton, but in this case we need unique
			std::shared_ptr<fmapp::PlayerTimePrinter> vis = injector.create<std::unique_ptr<fmapp::PlayerTimePrinter>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isWatchSet()) {
			std::shared_ptr<fmapp::SheetWatcher> vis = injector.create<std::unique_ptr<fmapp::SheetWatcher>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isUdpSet()) {
			std::shared_ptr<fmapp::Funkfeuer> vis = injector.create<std::unique_ptr<fmapp::Funkfeuer>>();
			loopVisitors.container.push_back(vis);
		}

		auto program = injector.create<SheetPlayerProgram*>();
		sheetWatcherHandlers->container.push_back(program);
		program->prepareEnvironment();
		auto result = program->execute();
		sheetWatcherHandlers->container.clear();
		return result;
	} catch (const fm::Exception &ex) {
		std::cerr << ex.toString() << std::endl;
		return 1;
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	
}