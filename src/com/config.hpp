#ifndef FM_CONFIG_HPP
#define FM_CONFIG_HPP

#include "units.hpp"
#include <vector>
#include <string>

#define SHEET_VERSION "0.1.62@9fd361f"
#define CHORD_DEF_EXTENSION  ".chords"
#define SHEET_TEMPLATE_DEF_EXTENSION ".template"
#define PITCHMAP_DEF_EXTENSION ".pitchmap"
#define LUA_DEF_EXTENSION ".lua"
#define SHEET_CONFIG ".config"
#define CONDUCTIONS_SHEET ".conductions"

#define FM_CHARSET "ISO-8859-1"
#define LIB_FLUIDSYNTH_FILENAME "libfluidsynth-2.dll"

namespace com {
    constexpr Ticks PPQ = 5000;
	constexpr int NotesPerOctave = 12;
    constexpr int SheetNavigationMaxJumps = 200;
    constexpr double DefaultTempo = 120.0;
    constexpr unsigned int FUNK_MAX_SIGNAL_LENGTH = 255;
    inline auto LibfluidSynthSearchPaths()
    {
        return std::vector<std::string> {"C:\\Windows\\System32"};
    }
}

#endif
