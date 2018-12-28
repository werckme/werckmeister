#include "SimpleGuitar.h"

namespace sheet {

 	SimpleGuitar::OctaveMap SimpleGuitar::createOctaveMap(const ChordDef &def) const
	{
		if (has7(def) && has9(def) && has11(def) && has13(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::IV, -1},
				{fm::degrees::VII, -1},
				{fm::degrees::II, 0},
				{fm::degrees::VI, 0},
			});
		}		
		if (has7(def) && has9(def) && has11(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::IV, -1},
				{fm::degrees::VII, -1},
				{fm::degrees::II, 0},
				{fm::degrees::V, 0},
			});
		}
		if (has7(def) && has9(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::III, -1},
				{fm::degrees::VII, -1},
				{fm::degrees::II, 0},
				{fm::degrees::V, 0},
			});
		}
		if (has7(def)) {
			return  OctaveMap({
				{fm::degrees::I, -1},
				{fm::degrees::V, -1},
				{fm::degrees::VII, -1},
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
		int transpose = 0;
		if (root > fm::notes::D && lowerRange) {
			transpose = -1;
		}
		for (const auto& degree : degreeIntervals) {
			auto interval = def.getIntervalBy(degree.pitch);
			if (!interval.valid()) {
				continue;
			}
			x.pitch = root + (interval.value % 12);
			auto octaveRange = octaves.equal_range(degree.pitch);
			auto octave = octaveRange.first;
			for(; octave != octaveRange.second; ++octave) { // some degrees may be twice (lower I & upper I)
				x.octave = octave->second + transpose;
				result.insert(x);
			}
		}

        return result;
	}

	void SimpleGuitar::setArguments(const Event::Args &args) 
	{
		if (args.size() > 1) {
				auto optionStr = getArgument<fm::String>(args, 1);
				if (optionStr == FM_STRING("lowerRange")) {
					lowerRange = true;
				}
				if (optionStr == FM_STRING("higherRange")) {
					lowerRange = false;
				}
			}
	}
}