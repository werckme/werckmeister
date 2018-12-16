#include <fm/werckmeister.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
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
#include "fmapp/os.hpp"
#include <thread>
#include <fm/config.hpp>
#include "fmapp/boostTimer.h"
#include <boost/format.hpp>

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_LIST "list"
#define ARG_MIDI_OUTPUT "device"
#define ARG_WATCH "watch"
#define ARG_CHORDS "sheet"
#define ARG_SECTION "style"
#define ARG_TEMPO "tempo"
#define ARG_INCLUDE "include"

typedef int MidiOutputId;

void onCompilerError(const std::exception &ex) {
	std::cout << ex.what() << std::endl;
}

void onCompilerError() {
	std::cout << "unkown error" << std::endl;
}

struct Settings {
	typedef boost::program_options::variables_map Variables;
	typedef boost::program_options::options_description OptionsDescription;
    typedef std::vector<std::string> Files;
	OptionsDescription optionsDescription;
	Variables variables;

	Settings(int argc, const char** argv) 
		: optionsDescription("Allowed options")
	{
		namespace po = boost::program_options;
		optionsDescription.add_options()
			(ARG_HELP, "produce help message")
			(ARG_INPUT, po::value<Files>(), "input files")
			(ARG_LIST, "list midi devices")
			(ARG_MIDI_OUTPUT, po::value<MidiOutputId>(), "select midi device (default = 0)")
			(ARG_WATCH, "checks the input file for changes and recompiles if any")
            (ARG_CHORDS, po::value<std::string>()->default_value("C"), "the sheet text, default is 'C'")
            (ARG_SECTION, po::value<std::string>(), "the style selection, e.g.: \"swing normal\"")
            (ARG_TEMPO, po::value<double>()->default_value(119), "the tempo in bpm, default is 119")
            (ARG_INCLUDE, po::value<std::string>(), "a file with sheet text to prepend (e.g. for instrument defs)")
			;
		po::positional_options_description p;
		p.add(ARG_INPUT, -1);
		po::store(po::command_line_parser(argc, argv).
			options(optionsDescription).positional(p).run(), variables);
		po::notify(variables);
	}

    std::string chords() const {
        return variables[ARG_CHORDS].as<std::string>();
    }

    bool hasStyle() const {
        return !!variables.count(ARG_SECTION);
    }

    std::string style() const {
        return variables[ARG_SECTION].as<std::string>();
    }

    double tempo() const {
        return variables[ARG_TEMPO].as<double>();
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
		return variables[ARG_INPUT].as<Files>();
	}

	bool device() const {
		return !!variables.count(ARG_MIDI_OUTPUT);
	}

	auto deviceId() const {
		return variables[ARG_MIDI_OUTPUT].as<MidiOutputId>();
	}

	bool watch() const {
		return !!variables.count(ARG_WATCH);
	}

    bool hasIncludeFile() {
        return !!variables.count(ARG_INCLUDE);
    }

    auto getIncludeFile() const {
        return variables[ARG_INCLUDE].as<std::string>();
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

void play(sheet::DocumentPtr document, fm::midi::MidiPtr midi, MidiOutputId midiOutput, const Settings &settings) {
	auto &player = fmapp::getMidiplayer();
	player.bpm(settings.tempo());
	auto output = findOutput(midiOutput);
	std::cout << "playing on: " << output.name;
	player.setOutput(output);
	player.midi(midi);
	player.play(0);
    auto end = midi->duration();
	bool playing = true;
	fmapp::os::setSigtermHandler([&playing]{
		playing = false;
		std::cout << std::endl << "stopped" << std::endl;
		std::cout.flush();
	});
	
#ifdef SHEET_USE_BOOST_TIMER
	std::thread boost_asio_([] {
		fmapp::BoostTimer::io_run();
	});
#endif
    std::cout.flush();
	while (playing) {
		auto elapsed = player.elapsed();    
		std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        if (elapsed > end) {
			player.play(0);
		}
	}
	std::cout << std::endl;
	player.stop();

#ifdef SHEET_USE_BOOST_TIMER
	fmapp::BoostTimer::io_stop();
	boost_asio_.join();
#endif

	player.panic();
}

fm::String getFileText(const std::string &path) {
    auto filestream = fm::getWerckmeister().openResource(path);
    fm::StreamBuffIterator begin(*filestream);
    fm::StreamBuffIterator end;
    return fm::String(begin, end);
}

fm::String getStyleText(const Settings &settings) 
{
    if (!settings.hasStyle()) {
        return FM_STRING("");
    }
    fm::StringStream ss;
    ss << "/style: " << fm::to_wstring(settings.style()) << "/";
    return ss.str();
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
			throw std::runtime_error("missing input files");
		}

		int midi_out = 0;
		if (settings.device()) {
			midi_out = settings.deviceId();
		}


		std::vector<std::string> infilesNarrow = settings.getInput();
        std::vector<fm::String> infiles(infilesNarrow.size());
        std::transform(infilesNarrow.begin(), infilesNarrow.end(), infiles.begin(), [](const auto &x){ return fm::to_wstring(x); });
        fm::String prependText = settings.hasIncludeFile() ? getFileText(settings.getIncludeFile()) : FM_STRING("");
        prependText += FM_STRING("\n") + getStyleText(settings);
        fm::String sheetText = prependText + FM_STRING("\n") + fm::to_wstring(settings.chords());

		auto doc = sheet::createDocumentByString(sheetText, infiles);
		auto midi = sheet::processFile(doc);
        play(doc, midi, midi_out, settings);

		return 0;
	}
	catch (const std::exception &ex)
	{
		onCompilerError(ex);
	}
	catch (...)
	{
		onCompilerError();
	}
	return -1;
}