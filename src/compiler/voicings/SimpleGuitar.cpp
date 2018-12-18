#include "SimpleGuitar.h"

namespace sheet {

 	SimpleGuitar::OctaveMap SimpleGuitar::createOctaveMap(const ChordDef &def) const
	{
		if (has7(def) && has9(def) && has11(def) && has13(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::IV, -1},
				{fm::degrees::VII, 0},
				{fm::degrees::II, 0},
				{fm::degrees::VI, 0},
				{fm::degrees::I, 1},
			});
		}		
		if (has7(def) && has9(def) && has11(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::IV, -1},
				{fm::degrees::VII, 0},
				{fm::degrees::II, 0},
				{fm::degrees::V, 0},
				{fm::degrees::I, 1},
			});
		}
		if (has7(def) && has9(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::III, -1},
				{fm::degrees::VII, 0},
				{fm::degrees::II, 0},
				{fm::degrees::V, 0},
				{fm::degrees::I, 1},
			});
		}
		if (has7(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::V, -1},
				{fm::degrees::VII, 0},
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