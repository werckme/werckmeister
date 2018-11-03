#include <fm/werckmeister.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "compiler/parser.h"

void processFile(const std::string &file)
{
	sheet::compiler::DocumentParser docparser;
	auto doc = docparser.parse(fm::to_wstring(file));
}


int main(int argc, const char** argv)
{
	try {
		typedef std::vector<std::string> Filenames;
		namespace po = boost::program_options;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("input-file", po::value<Filenames>(), "input file")
			;
		po::positional_options_description p;
		p.add("input-file", -1);
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
		po::notify(vm);
		
		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 1;
		}
		
		if (vm.count("input-file") > 0) {
			processFile(vm["input-file"].as<Filenames>()[0]);
		}
		return 0;
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unkown error" << std::endl;
	}
	return -1;
}