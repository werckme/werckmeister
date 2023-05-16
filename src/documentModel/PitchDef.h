#pragma once

#include <com/common.hpp>
#include <com/units.hpp>
#include <com/config.hpp>
#include <vector>
#include <set>
#include <tuple>
#include <functional>

namespace documentModel
{

	namespace
	{
		std::hash<com::String> hash_fn;
	}

	struct PitchDef
	{
		typedef com::Pitch Pitch;
		typedef int Octave;
		enum
		{
			NoPitch = -1,
			DefaultOctave = 0,
		};
		Pitch pitch = NoPitch;
		Pitch originalDegree = NoPitch;
		Octave octave = DefaultOctave;
		com::String alias;
		bool forceDegree = false;
		PitchDef(const com::String &alias) : alias(alias) {}
		PitchDef(Pitch p = NoPitch, Octave o = DefaultOctave) : pitch(p), octave(o) {}
		int id() const
		{
			if (!alias.empty())
			{
				return static_cast<int>(hash_fn(alias));
			}
			return pitch + (octave * com::NotesPerOctave);
		}
		bool operator<(const PitchDef &b) const
		{
			return id() < b.id();
		}
		bool operator>(const PitchDef &b) const
		{
			return id() > b.id();
		}
		bool operator==(const PitchDef &b) const
		{
			return id() == b.id();
		}
		bool operator!=(const PitchDef &b) const
		{
			return id() != b.id();
		}
	};
}
