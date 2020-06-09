#include <boost/di.hpp>
#include "SheetCompilerProgram.h"
#include <parser/parser.h>

int main(int argc, const char** argv)
{
	namespace di = boost::di;
	auto injector = di::make_injector(
		di::bind<sheet::compiler::IDocumentParser>().to<sheet::compiler::DocumentParser>()
	);
  	auto program = injector.create<SheetCompilerProgram>();
	program.execute();
}