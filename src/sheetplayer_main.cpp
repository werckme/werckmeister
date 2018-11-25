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
#include <thread>
#include <fm/config.hpp>
#include <ctime>
#include "fmapp/boostTimer.h"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_LIST "list"
#define ARG_MIDI_OUTPUT "device"
#define ARG_LOOP "loop"
#define ARG_BEGIN "begin"
#define ARG_END "end"
#define ARG_WATCH "watch"

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
			(ARG_WATCH, "checks the input file for changes and recompiles if any")
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

	bool watch() const {
		return !!variables.count(ARG_WATCH);
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

auto getTimestamp(const std::string input) {
	auto path = boost::filesystem::path(input);
	return boost::filesystem::last_write_time(path);
}

void play(fm::midi::MidiPtr midi, MidiOutputId midiOutput, fm::Ticks begin, fm::Ticks end, const Settings &settings) {
	auto &player = fmapp::getMidiplayer();
	auto output = findOutput(midiOutput);
	std::cout << "playing on: " << output.name << std::endl;
	player.setOutput(output);
	player.midi(midi);
	player.play(begin);
	bool playing = true;
	bool watch = settings.watch();
	auto inputfile = settings.getInput();
	auto timestamp = getTimestamp(inputfile);

	fmapp::os::setSigtermHandler([&playing]{
		playing = false;
		std::cout << "stopped" << std::endl;
		std::cout.flush();
	});
	
#ifdef SHEET_USE_BOOST_TIMER
	std::thread boost_asio_([] {
		fmapp::BoostTimer::io_run();
	});
#endif

	while (playing) {
		std::this_thread::sleep_for( std::chrono::milliseconds(10) );
		if (watch) {
			auto newTimestamp = getTimestamp(inputfile);
			if (timestamp != newTimestamp) {
				timestamp = newTimestamp;
				auto pos = player.elapsed();
				player.stop();
				try {
					player.midi(sheet::processFile(inputfile));
				}
				catch (const std::exception &ex)
				{
					std::cout << ex.what() << std::endl;
					break;
				}
				catch (...)
				{
					std::cout << "unkown error" << std::endl;
					break;
				}
				player.play(pos);
				continue;
			}
			
		}
		if (player.elapsed() > end) {
			if (!settings.loop()) {
				break;
			}
			player.play(begin);
		}
	}
	player.stop();

#ifdef SHEET_USE_BOOST_TIMER
	fmapp::BoostTimer::io_stop();
	boost_asio_.join();
#endif
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
		fm::Ticks begin = 0;

		auto end = midi->duration();
		if (settings.begin()) {
			begin = fm::Ticks((double)fm::PPQ * settings.getBegin());
		}

		if (settings.end()) {
			end = fm::Ticks((double)fm::PPQ * settings.getEnd());
		}

		if (begin >= end) {
			throw std::runtime_error("invalid begin/end range");
		}


		play(midi, midi_out, begin, end, settings);

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