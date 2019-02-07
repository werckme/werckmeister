#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <fm/config.hpp>
#include <vector>
#include <set>
#include <tuple>
#include <functional>
#include <fm/exception.hpp>

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
		bool operator>(const PitchDef& b) const
		{
			return id() > b.id();
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
			TiedDegree,
			Note,
			TiedNote,
			Chord,
			EOB, // End of Bar aka. Bar Line
			Meta,
			Expression,
			NumEvents
		};
		typedef fm::Ticks Duration;
		typedef std::set<PitchDef> Pitches;
		typedef std::vector<fm::String> Args;
		typedef fm::String Options;
		typedef std::tuple<PitchDef::Pitch, Options> ChordElements;
		typedef long double Multiplicator;
		ChordElements chordElements() const;
		fm::String chordDefName() const;
		Multiplicator multiplicator = 1; // to multiplicate with bar length e.g.: | C(1) | C(0.5) C(0.5) |		
		Pitches pitches;
		Type type = Unknown;
		Duration duration = NoDuration;
		/**
		 *  can be for instance a meta event command or a chordname
		 */
		fm::String stringValue;
		Args metaArgs;

		bool isTimeConsuming() const {
			return type == Rest || type == Note || type == Degree || type == TiedNote || type == Chord;
		}
	};

	struct ChordEvent : Event {
		fm::String chordName;
		typedef fm::String Options;
		typedef std::tuple<PitchDef::Pitch, Options> ChordElements;
		typedef long double Multiplicator;
		ChordElements chordElements() const { return ChordElements(); }
		fm::String chordDefName() const { return fm::String(); }
		Multiplicator multiplicator = 1; // to multiplicate with bar length e.g.: | C(1) | C(0.5) C(0.5) |
	};
}

#endif