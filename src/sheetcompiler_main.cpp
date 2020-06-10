#include <boost/di.hpp>
#include "SheetCompilerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <fm/ConsoleLogger.h>
#include <iostream>
#include <memory>

struct ProgramOptionParserWrapper {
	ProgramOptionParserWrapper(std::shared_ptr<CompilerProgramOptions> options) : options(options) {}
	std::shared_ptr<CompilerProgramOptions> options;
	void parse(int argc, const char** argv) {
		options->parseProgrammArgs(argc, argv);
	}
};

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;

	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>().to<cp::DocumentParser>()
		, di::bind<fm::Werckmeister>().to<fm::Werckmeister>()
		, di::bind<ICompilerProgramOptions>().to<CompilerProgramOptions>().in(di::singleton)
		, di::bind<fm::ILogger>().to<fm::ConsoleLogger>().in(di::singleton)
	);
	auto optionParser = injector.create<ProgramOptionParserWrapper>();
	auto program = injector.create<SheetCompilerProgram>();
	try {
		optionParser.parse(argc, argv);
		program.prepareEnvironment();
		program.execute();
	} catch (const std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}