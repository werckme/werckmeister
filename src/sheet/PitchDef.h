#ifndef PITCHDEF_H
#define PITCHDEF_H

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
}

#endif