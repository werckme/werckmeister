#include <boost/di.hpp>
#include <iostream>
#include <memory>
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
	);
	auto program = injector.create<SheetPlayerProgram>();
	program.prepareEnvironment();
	return program.execute();
}