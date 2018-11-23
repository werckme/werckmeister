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
#include "fmapp/os.hpp"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_LIST "list"
#define ARG_MIDI_OUTPUT "device"
#define ARG_LOOP "loop"
#define ARG_BEGIN "begin"
#define ARG_END "end"

typedef int MidiOutputId;

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
			(ARG_LIST, "list midi devices")
			(ARG_LOOP, "activates playback loop")
			(ARG_BEGIN, po::value<double>(), "start postition in quarter notes. E.g.: 1.2")
			(ARG_END, po::value<double>(), "estop postition in quarter notes. E.g.: 1.2")
			(ARG_MIDI_OUTPUT, po::value<MidiOutputId>(), "select midi device (default = 0)")
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

	bool listDevices() const {
		return !!variables.count(ARG_LIST);
	}

	bool input() const {
		return !!variables.count(ARG_INPUT);
	}

	auto getInput() const {
		return variables[ARG_INPUT].as<std::string>();
	}

	bool device() const {
		return !!variables.count(ARG_MIDI_OUTPUT);
	}

	auto deviceId() const {
		return variables[ARG_MIDI_OUTPUT].as<MidiOutputId>();
	}

	bool loop() const {
		return !!variables.count(ARG_LOOP);
	}

	bool begin() const {
		return !!variables.count(ARG_BEGIN);
	}

	auto getBegin() const {
		return variables[ARG_BEGIN].as<double>();
	}

	bool end() const {
		return !!variables.count(ARG_END);
	}

	auto getEnd() const {
		return variables[ARG_END].as<double>();
	}

};

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

void play(fm::midi::MidiPtr midi, MidiOutputId midiOutput, const Settings &settings) {
	auto &player = fmapp::getMidiplayer();
	auto output = findOutput(midiOutput);
	auto end = midi->duration();
	fm::Ticks begin = 0;
	std::cout << "playing on: " << output.name << std::endl;
	player.setOutput(output);
	player.midi(midi);
	player.play();
	bool playing = true;
	
	if (settings.begin()) {
		begin = fm::Ticks((double)fm::PPQ * settings.getBegin());
		player.seek(begin);
	}

	if (settings.end()) {
		end = fm::Ticks((double)fm::PPQ * settings.getEnd());
	}

	if (begin >= end) {
		throw std::runtime_error("invalid begin/end range");
	}

	fmapp::os::setSigtermHandler([&playing]{
		playing = false;
		std::cout << "stopped" << std::endl;
		std::cout.flush();
	});

	while (playing) {
		std::this_thread::sleep_for( std::chrono::milliseconds(10) );
		if (player.elapsed() > end) {
			if (!settings.loop()) {
				break;
			}
			player.seek(begin);
		}
	}
	player.stop();
}

int main(int argc, const char** argv)
{
	try {

		Settings settings(argc, argv);
				
		if (settings.help()) {
			std::cout << settings.optionsDescription << "\n";
			return 1;
		}
		
		if (settings.listDevices()) {
			return listDevices();
		}

		if (!settings.input()) {
			throw std::runtime_error("missing input file");
		}

		int midi_out = 0;
		if (settings.device()) {
			midi_out = settings.deviceId();
		}

		std::string infile = settings.getInput();
		auto midi = sheet::processFile(infile);
		play(midi, midi_out, settings);


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