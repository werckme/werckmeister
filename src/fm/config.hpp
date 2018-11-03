#ifndef FM_CONFIG_HPP
#define FM_CONFIG_HPP

#include "units.hpp"

#define FETZER_TO_STRING(x) #x

#ifndef GIT_VERSION
#define FETZER_VERSION "fetzer unknown version"
#else
#define FETZER_VERSION "libfetzer: " GIT_VERSION
#endif

#define CHORD_DEF_EXTENSION  ".chords"
#define STYLE_DEF_EXTENSION ".style"

namespace fm {
    const Ticks PPQ = 500;
	const int NotesPerOctave = 12;
}

#endif
