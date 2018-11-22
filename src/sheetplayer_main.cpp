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
#include "fmapp/midiplayer.h"
#include <algorithm>
#include <thread>
#include <chrono>

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_LIST "list"
#define ARG_MIDI_OUTPUT "device"

typedef int MidiOutputId;

int listDevices() {
	auto &player = fmapp::getMidiplayer();
	auto outputs = player.getOutputs();
	for (const auto &output : outputs) {
		std::cout << output.id << ": " << output.name << std::endl;
	}
	return 0;
}

fmapp::Midiplayer::Output findOutput(MidiOutputId id) 
{
	auto &player = fmapp::getMidiplayer();
	auto outputs = player.getOutputs();
	auto it = std::find_if(outputs.begin(), outputs.end(), [id](const auto &x) { return x.id == id; });
	if (it == outputs.end()) {
		throw std::runtime_error("device not found: " + std::to_string(id));
	}
	return *it;
}

void play(fm::midi::MidiPtr midi, MidiOutputId midiOutput) {
	auto &player = fmapp::getMidiplayer();
	auto output = findOutput(midiOutput);
	std::cout << "playing on: " << output.name << std::endl;
	player.setOutput(output);
	player.midi(midi);
	player.play();
	while (true) {
		std::this_thread::sleep_for( std::chrono::milliseconds(10) );
	}
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
			(ARG_LIST, "list midi devices")
			(ARG_MIDI_OUTPUT, po::value<MidiOutputId>(), "select midi device (default = 0)")
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
		
		if (vm.count(ARG_LIST)) {
			return listDevices();
		}

		if (vm.count(ARG_INPUT) == 0) {
			throw std::runtime_error("missing input file");
		}

		int midi_out = 0;
		if (vm.count(ARG_MIDI_OUTPUT)) {
			midi_out = vm[ARG_MIDI_OUTPUT].as<MidiOutputId>();
		}

		std::string infile = vm[ARG_INPUT].as<std::string>();
		auto midi = sheet::processFile(infile);
		play(midi, midi_out);


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