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


struct Settings {
	typedef boost::program_options::variables_map Variables;
	typedef boost::program_options::options_description OptionsDescription;
	OptionsDescription optionsDescription;
	Variables variables;

	Settings(int argc, const char** argv)
		: optionsDescription("Allowed options")
	{
		namespace po = boost::program_options;
		optionsDescription.add_options()
			(ARG_HELP, "produce help message")
			(ARG_INPUT, po::value<std::string>(), "input file")
			(ARG_OUTPUT, po::value<std::string>(), "output file")
			;
		po::positional_options_description p;
		p.add(ARG_INPUT, -1);
		po::store(po::command_line_parser(argc, argv).
			options(optionsDescription).positional(p).run(), variables);
		po::notify(variables);
	}


	bool help() const {
		return !!variables.count(ARG_HELP);
	}

	bool input() const {
		return !!variables.count(ARG_INPUT);
	}

	auto getInput() const {
		return variables[ARG_INPUT].as<std::string>();
	}

	bool output() const {
		return !!variables.count(ARG_OUTPUT);
	}

	auto getOutput() const {
		return variables[ARG_OUTPUT].as<std::string>();
	}

};

void saveMidi(fm::midi::MidiPtr midi, const std::string &filename)
{
	std::ofstream fstream(filename, std::ios::binary);
	midi->write(fstream);
	fstream.flush();
}



int main(int argc, const char** argv)
{
	try {
		Settings settings(argc, argv);
		
		if (settings.help()) {
			std::cout << settings.optionsDescription << "\n";
			return 1;
		}
		
		if (!settings.input()) {
			throw std::runtime_error("missing input file");
		}

		std::string infile = settings.getInput();
		auto midi = sheet::processFile(infile);

		std::string outfile = boost::filesystem::path(infile).filename().string() + ".mid";
		if (settings.output()) {
			outfile = settings.getOutput();
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