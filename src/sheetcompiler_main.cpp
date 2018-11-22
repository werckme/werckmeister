#include <fm/werckmeister.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "compiler/MidiContext.h"
#include <fstream>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "sheet.h"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_OUTPUT "output"

void saveMidi(fm::midi::MidiPtr midi, const std::string &filename)
{
	std::ofstream fstream(filename, std::ios::binary);
	midi->write(fstream);
	fstream.flush();
}



int main(int argc, const char** argv)
{
	try {
		typedef std::vector<std::string> Filenames;
		namespace po = boost::program_options;
		po::options_description desc("Allowed options");
		desc.add_options()
			(ARG_HELP, "produce help message")
			(ARG_INPUT, po::value<std::string>(), "input file")
			(ARG_OUTPUT, po::value<std::string>(), "output file")
			;
		po::positional_options_description p;
		p.add(ARG_INPUT, -1);
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
		po::notify(vm);
		
		if (vm.count(ARG_HELP)) {
			std::cout << desc << "\n";
			return 1;
		}
		
		if (vm.count(ARG_INPUT) == 0) {
			throw std::runtime_error("missing input file");
		}

		std::string infile = vm[ARG_INPUT].as<std::string>();
		auto midi = sheet::processFile(infile);

		std::string outfile = boost::filesystem::path(infile).filename().string() + ".mid";
		if (vm.count(ARG_OUTPUT) > 0) {
			outfile = vm[ARG_OUTPUT].as<std::string>();
		}
		saveMidi(midi, outfile);
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