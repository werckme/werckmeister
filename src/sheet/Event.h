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
#include "ASheetObject.hpp"

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
		bool forceDegree = false;
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

	struct Event : public ASheetObjectWithSourceInfo {
		typedef ASheetObjectWithSourceInfo Base;
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
			Repeat,
			TiedRepeat,
			Chord,
			EOB, // End of Bar aka. Bar Line
			Meta,
			Expression,
			PitchBend,
			NumEvents
		};
		typedef fm::Ticks Duration;
		typedef std::set<PitchDef> Pitches;
		typedef std::vector<fm::String> Args;
		typedef fm::String Options;
		typedef std::tuple<PitchDef::Pitch, Options> ChordElements;
		
		ChordElements chordElements() const;
		fm::String chordDefName() const;	
		Pitches pitches;
		Type type = Unknown;
		Duration duration = NoDuration;
		/**
		 *  can be for instance a meta event command or a chordname
		 */
		fm::String stringValue;
		Args metaArgs;
		/**
		 * the event velocity value in a range 0..1, will be set during rendering
		 */
		double velocity = 0;
		fm::Ticks offset = 0;
		double pitchBendValue = 0;
		bool isTimeConsuming() const {
			return type == Rest 
				|| type == Note 
				|| type == Degree 
				|| type == TiedNote 
				|| type == Chord 
				|| type == TiedDegree
				|| type == Repeat
				|| type == TiedRepeat;
		}
		
		bool isRepeat() const {
			return type == Repeat
				|| type == TiedRepeat;
		}

		bool isAbsoluteNote() const {
			return type == Note || type == TiedNote;
		}

		bool isRelativeDegree() const {
			return type == Degree || type == TiedDegree;
		}

		bool isTied() const {
			return type == TiedNote || type == TiedDegree;
		}

		void isTied(bool val);

		bool isPitchBend() const {
			return type == PitchBend;
		}

		fm::String toString() const;
	};
}

#endif