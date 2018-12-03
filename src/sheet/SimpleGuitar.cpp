#include "SimpleGuitar.h"
#include <algorithm>

namespace sheet {

	bool SimpleGuitar::has7(const ChordDef &def) const
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::VII; });
	}
	bool SimpleGuitar::has9(const ChordDef &def) const
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::II; });
	}
	bool SimpleGuitar::has11(const ChordDef &def) const
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::IV; });
	}
	bool SimpleGuitar::has13(const ChordDef &def) const
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::VI; });
	}

 	SimpleGuitar::OctaveMap SimpleGuitar::createOctaveMap(const ChordDef &def) const
	{
		if (has7(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::VII, -1},
				{fm::degrees::I, 0},
				{fm::degrees::III, 0},
				{fm::degrees::V, 0},
				{fm::degrees::I, 1},
			});
		}
		return  OctaveMap({
			{fm::degrees::I, -1},
			{fm::degrees::V, -1},
			{fm::degrees::I, 0},
			{fm::degrees::III, 0},
			{fm::degrees::V, 0},
			{fm::degrees::I, 1},
		});
	}

	SimpleGuitar::Pitches SimpleGuitar::get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo& timeInfo)
	{
		if (degreeIntervals.size() < 3) {
			return Base::get(chord, def, degreeIntervals, timeInfo);
		}

		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		auto octaves = createOctaveMap(def);
		for (const auto& degree : degreeIntervals) {
			const auto *interval = def.getIntervalBy(degree.pitch);
			if (!interval) {
				continue;
			}
			x.pitch = (root-1) + interval->value;
			auto octaveRange = octaves.equal_range(degree.pitch);
			auto octave = octaveRange.first;
			for(; octave != octaveRange.second; ++octave) {
				x.octave = octave->second;
				result.insert(x);
			}
		}

        return result;
	}
}