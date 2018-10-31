#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <vector>

namespace sheet {

	struct PitchDef {
		typedef int Pitch;
		typedef int Octave;
		enum {
			NoPitch = -1,
			DefaultOctave = 0,
		};
		Pitch pitch = NoPitch;
		Octave octave = DefaultOctave;
		PitchDef(Pitch p = NoPitch, Octave o = DefaultOctave) : pitch(p), octave(o) {}
	};

	struct Event {
		enum {
			NoDuration = 0,
		};
		enum Type { 
			Unknown,
			Rest,
			Degree, 
			Absolute, 
			EOB, // End of Bar aka. Bar Line
			Meta
		};
		typedef fm::Ticks Duration;
		typedef std::vector<PitchDef> Pitches;
		Pitches pitches;
		Type type = Unknown;
		Duration duration = NoDuration;
		fm::String metaCommand;
		fm::String metaArgs;
	};
}

#endif