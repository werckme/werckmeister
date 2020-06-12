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

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
	programOptionsPtr->parseProgrammArgs(argc, argv);

	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>().to<cp::DocumentParser>()
		, di::bind<cp::ICompiler>().to<cp::Compiler>()
		, di::bind<cp::ISheetTemplateRenderer>().to<cp::SheetTemplateRenderer>()
		, di::bind<cp::ASheetEventRenderer>().to<cp::SheetEventRenderer>()
		, di::bind<cp::IContext>().to<cp::MidiContext>()
		, di::bind<cp::IPreprocessor>().to<cp::Preprocessor>()
		, di::bind<fm::Werckmeister>().to<fm::Werckmeister>()
		, di::bind<ICompilerProgramOptions>().to(programOptionsPtr)
		, di::bind<fm::ILogger>().to<fm::ConsoleLogger>().in(di::singleton)
	);
	auto program = injector.create<SheetCompilerProgram>();
	try {
		program.prepareEnvironment();
		program.execute();
	} catch (const std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}