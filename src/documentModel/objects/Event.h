#pragma once

#include <com/common.hpp>
#include <com/units.hpp>
#include <com/config.hpp>
#include <vector>
#include <set>
#include <tuple>
#include <functional>
#include <com/exception.hpp>
#include "ASheetObjectWithSourceInfo.h"
#include <documentModel/Argument.h>
#include <documentModel/PitchDef.h>

namespace documentModel {

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
		typedef com::Ticks Duration;
		typedef std::vector<PitchDef> Pitches;
		typedef std::vector<documentModel::Argument> Args;
		typedef std::vector<Event> EventGroup;
		typedef std::set<com::String> Tags;
		typedef com::String Options;
		typedef std::tuple<PitchDef::Pitch, Options> ChordElements;
		
		ChordElements chordElements() const;
		com::String chordDefName() const;	
		Pitches pitches;
		Type type = Unknown;
		Duration duration = NoDuration;
		/**
		 * duration of this event and all following tied events
		 */
		Duration tiedDuration = NoDuration;
		/**
		 * duration of the total tied events chain
		 **/
		Duration tiedDurationTotal = NoDuration;
		/**
		 *  can be for instance a meta event command or a chordname
		 */
		com::String stringValue;
		Args metaArgs;
		Tags tags;
		EventGroup eventGroup;
		/**
		 * the event velocity value in a range 0..1, will be set during rendering
		 */
		double velocity = 0;
		com::Ticks offset = 0;
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

		com::String toString() const;
	};
}

