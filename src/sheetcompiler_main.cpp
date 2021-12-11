#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetCompilerProgram.h"
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
#include <sheet/Document.h>
#include <com/DefinitionsServer.h>
#include <com/midi.hpp>
#include <app/MidiFileWriter.h>
#include <app/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <app/TimelineVisitor.hpp>
#include <compiler/SheetNavigator.h>
#include <conductor/ConductionsPerformer.h>
#include "FactoryConfig.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

typedef sheet::compiler::EventLogger<com::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<com::ConsoleLogger> WarningsCollectorWithConsoleLogger;


int main(int argc, const char** argv)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	namespace co = sheet::conductor;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
	try {
		programOptionsPtr->parseProgrammArgs(argc, argv);
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = com::getWerckmeister().createMidi();
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
		, di::bind<co::IConductionsPerformer>()		.to<co::ConductionsPerformer>()		.in(di::singleton)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<com::IDefinitionsServer>()		.to<com::DefinitionsServer>()		.in(di::singleton)
		, di::bind<com::midi::Midi>()				.to(midiFile)
		, di::bind<app::IDocumentWriter>()		.to([&](const auto &injector) -> app::IDocumentWriterPtr
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::shared_ptr<app::JsonWriter>>();
			}
			return injector.template create<std::shared_ptr<app::MidiFileWriter>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<app::DefaultTimeline>>();
			}
			return injector.template create< std::shared_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<com::ILogger>()					.to([&](const auto &injector) -> com::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
	);
	sheet::FactoryConfig factory(injector);
	factory.init();
	auto program = injector.create<SheetCompilerProgram>();
	program.prepareEnvironment();
	return program.execute();
}