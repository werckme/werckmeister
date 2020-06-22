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
#include "ASheetObjectWithSourceInfo.h"
#include <sheet/Argument.h>
#include <sheet/PitchDef.h>

namespace sheet {

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
			Group,
			NumEvents
		};
		typedef fm::Ticks Duration;
		typedef std::vector<PitchDef> Pitches;
		typedef std::vector<sheet::Argument> Args;
		typedef std::vector<Event> EventGroup;
		typedef std::set<fm::String> Tags;
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
		Tags tags;
		EventGroup eventGroup;
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
				|| type == TiedRepeat
				|| type == Group;
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