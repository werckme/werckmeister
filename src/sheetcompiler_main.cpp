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
#include "fmapp/json.hpp"
#include <fstream>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "sheet.h"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_OUTPUT "output"
#define ARG_MODE "mode"

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
			(ARG_MODE, po::value<std::string>(), "mode: normal or json; in JSON mode the input and output will be implemented using JSON strings")
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

	bool isJsonMode() const {
		return variables[ARG_MODE].as<std::string>() == "json";
	}

};

void saveMidi(fm::midi::MidiPtr midi, const std::string &filename)
{
	std::ofstream fstream(filename, std::ios::binary);
	midi->write(fstream);
	fstream.flush();
}


void printWarnings(const sheet::Warnings &warnings) 
{
	if (warnings.empty()) {
		return;
	}
	for (const auto &warning : warnings) {
		std::cout << warning << std::endl;
	}
}

void prepareJSONMode(const std::string &jsonData) {
	fmapp::JsonReader jsonReader;
	auto vfiles = jsonReader.readVirtualFS(jsonData);
	for(const auto &vfile : vfiles) {
		std::cout << vfile.path <<  std::endl;
		std::cout << "    " << vfile.data <<  std::endl;
	}
}

int main(int argc, const char** argv)
{
	try {
		Settings settings(argc, argv);
		
		if (settings.help()) {
			std::cout << settings.optionsDescription << "\n";
			return 1;
		}
		
		if (settings.isJsonMode() && settings.input()) {
			prepareJSONMode(settings.getInput());
			return 0;
		}
		else if (!settings.input()) {
			throw std::runtime_error("missing input file");
		}

		std::string infile = settings.getInput();
		sheet::Warnings warnings;
		auto midi = sheet::processFile(infile, warnings);
		printWarnings(warnings);

		std::string outfile = boost::filesystem::path(infile).filename().string() + ".mid";
		if (settings.output()) {
			outfile = settings.getOutput();
		}
		saveMidi(midi, outfile);
		return 0;
	}
	catch (const fm::Exception &ex)
	{
		sheet::onCompilerError(ex);
	}
	catch (const std::exception &ex)
	{
		sheet::onCompilerError(ex);
	}
	catch (...)
	{
		sheet::onCompilerError();
	}
	return -1;
}