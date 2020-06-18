#include <boost/di.hpp>
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

typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<PlayerProgramOptions>();

	try {
		programOptionsPtr->parseProgrammArgs(argc, argv);
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	fmapp::SheetWatcherHandlersPtr sheetWatcherHandlers = std::make_shared<fmapp::SheetWatcherHandlers>();
	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isJsonModeSet();
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isValidateMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()			.to<cp::DocumentParser>()			.in(di::singleton)
		, di::bind<cp::ICompiler>()					.to<cp::Compiler>()					.in(di::singleton)
		, di::bind<cp::ISheetTemplateRenderer>()	.to<cp::SheetTemplateRenderer>()	.in(di::singleton)
		, di::bind<cp::ASheetEventRenderer>()		.to<cp::SheetEventRenderer>()		.in(di::singleton)
		, di::bind<cp::IContext>()					.to<cp::MidiContext>()				.in(di::singleton)
		, di::bind<cp::IPreprocessor>()				.to<cp::Preprocessor>()				.in(di::singleton)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()		.to<fm::DefinitionsServer>()		.in(di::singleton)
		, di::bind<fm::midi::Midi>()				.to(midiFile)
		, di::bind<fmapp::SheetWatcherHandlers>()	.to(sheetWatcherHandlers)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr
		{
			return injector.template create<std::shared_ptr<fmapp::MidiPlayer>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<fmapp::DefaultTimeline>>();
			}
			return injector.template create< std::shared_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<fm::ILogger>()					.to([&](const auto &injector) -> fm::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
		, di::bind<fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr>>().to([&](const auto &injector) {
			fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr> wrapper;
			if (!programOptionsPtr->isNoTimePrintSet()) {
				wrapper.container.emplace_back( injector.template create< std::shared_ptr<fmapp::PlayerTimePrinter>>() );
			}
			if (programOptionsPtr->isWatchSet()) {
				wrapper.container.emplace_back( injector.template create< std::shared_ptr<fmapp::SheetWatcher>>() );
			}
			return wrapper;
		})
	);
	auto program = injector.create<SheetPlayerProgram>();
	sheetWatcherHandlers->container.push_back(&program);
	program.prepareEnvironment();
	return program.execute();
}