#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetCompilerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <fm/ConsoleLogger.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <sheet/Document.h>

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
	programOptionsPtr->parseProgrammArgs(argc, argv);

	auto documentPtr = std::make_shared<sheet::Document>();

	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()			.to<cp::DocumentParser>()			.in(di::singleton)
		, di::bind<cp::ICompiler>()					.to<cp::Compiler>()					.in(di::singleton)
		, di::bind<cp::ISheetTemplateRenderer>()	.to<cp::SheetTemplateRenderer>()	.in(di::singleton)
		, di::bind<cp::ASheetEventRenderer>()		.to<cp::SheetEventRenderer>()		.in(di::singleton)
		, di::bind<cp::IContext>()					.to<cp::MidiContext>()				.in(di::singleton)
		, di::bind<cp::IPreprocessor>()				.to<cp::Preprocessor>()				.in(di::singleton)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<fm::ILogger>()					.to<fm::ConsoleLogger>()			.in(di::singleton)
	);
	auto program = injector.create<SheetCompilerProgram>();
	try {
		program.prepareEnvironment();
		program.execute();
	} catch (const std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}