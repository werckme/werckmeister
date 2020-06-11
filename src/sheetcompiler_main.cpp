#include <boost/di.hpp>
#include "SheetCompilerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <fm/ConsoleLogger.h>
#include <iostream>
#include <memory>


int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<CompilerProgramOptions>();
	programOptionsPtr->parseProgrammArgs(argc, argv);

	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>().to<cp::DocumentParser>()
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