#ifndef FM_CONFIG_HPP
#define FM_CONFIG_HPP

#include "units.hpp"

#define SHEET_VERSION "1c9cfcf"
#define CHORD_DEF_EXTENSION  ".chords"
#define STYLE_DEF_EXTENSION ".style"
#define USE_WINDOWS_MME_TIMER

#define FM_CHARSET "ISO-8859-1"

namespace fm {
    const Ticks PPQ = 500;
	const int NotesPerOctave = 12;
}

#endif
