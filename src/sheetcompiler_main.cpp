#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetCompilerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <CompilerProgramOptions.h>
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
#include <fmapp/MidiFileWriter.h>
#include <fmapp/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <fmapp/TimelineVisitor.hpp>
#include <compiler/SheetNavigator.h>

typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();

	try {
		programOptionsPtr->parseProgrammArgs(argc, argv);
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isJsonModeSet();
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()			.to<cp::DocumentParser>()			.in(di::singleton)
		, di::bind<cp::ICompiler>()					.to<cp::Compiler>()					.in(di::singleton)
		, di::bind<cp::ISheetTemplateRenderer>()	.to<cp::SheetTemplateRenderer>()	.in(di::singleton)
		, di::bind<cp::ASheetEventRenderer>()		.to<cp::SheetEventRenderer>()		.in(di::singleton)
		, di::bind<cp::IContext>()					.to<cp::MidiContext>()				.in(di::singleton)
		, di::bind<cp::IPreprocessor>()				.to<cp::Preprocessor>()				.in(di::singleton)
		, di::bind<cp::ISheetNavigator>()			.to<cp::SheetNavigator>()			.in(di::singleton)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()		.to<fm::DefinitionsServer>()		.in(di::singleton)
		, di::bind<fm::midi::Midi>()				.to(midiFile)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr 
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::shared_ptr<fmapp::JsonWriter>>();
			}
			return injector.template create<std::shared_ptr<fmapp::MidiFileWriter>>();
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
	auto program = injector.create<SheetCompilerProgram>();
	program.prepareEnvironment();
	return program.execute();
}