#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <fm/config.hpp>
#include <vector>
#include <set>

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
		int id() const 
		{
			return pitch + (octave * fm::NotesPerOctave);
		}
		bool operator<(const PitchDef& b) const
		{
			return id() < b.id();
		}
	};

	struct Event {
		enum {
			NoDuration = 0,
		};
		enum Type { 
			Unknown,
			Rest,
			Degree, 
			Note,
			Chord,
			EOB, // End of Bar aka. Bar Line
			Meta,
			NumEvents
		};
		typedef fm::Ticks Duration;
		typedef std::set<PitchDef> Pitches;
		typedef std::vector<fm::String> Args;
		Pitches pitches;
		Type type = Unknown;
		Duration duration = NoDuration;
		fm::String metaCommand;
		Args metaArgs;
	};

	struct ChordEvent : Event {
		fm::String chordName;
	};


}

#endif