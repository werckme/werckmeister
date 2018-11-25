#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <fm/config.hpp>
#include <vector>
#include <set>
#include <tuple>
#include <functional>

namespace sheet {

	namespace {
		std::hash<fm::String> hash_fn;
	}

	struct PitchDef {
		typedef fm::Pitch Pitch;
		typedef int Octave;
		enum {
			NoPitch = -1,
			DefaultOctave = 0,
		};
		Pitch pitch = NoPitch;
		Octave octave = DefaultOctave;
		fm::String alias;
		PitchDef(const fm::String &alias) : alias(alias) {}
		PitchDef(Pitch p = NoPitch, Octave o = DefaultOctave) : pitch(p), octave(o) {}
		int id() const 
		{
			if (!alias.empty()) {
				return static_cast<int>(hash_fn(alias));
			}
			return pitch + (octave * fm::NotesPerOctave);
		}
		bool operator<(const PitchDef& b) const
		{
			return id() < b.id();
		}
	};

	struct AliasPitch : public PitchDef {};

	struct Event {
		enum {
			NoDuration = 0,
		};
		enum Type { 
			Unknown,
			Rest,
			Degree, 
			Note,
			TiedNote,
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

		bool isTimeConsuming() const {
			return type == Rest || type == Note || type == Degree || type == TiedNote || type == Chord;
		}
	};

	struct ChordEvent : Event {
		fm::String chordName;
		typedef fm::String Options;
		typedef std::tuple<PitchDef::Pitch, Options> ChordElements;
		ChordElements chordElements() const;
		fm::String chordDefName() const;
	};
}

#endif