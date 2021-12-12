#include "SimpleGuitar.h"
#include <com/tools.h>

namespace compiler
{

	SimpleGuitar::OctaveMap SimpleGuitar::createOctaveMap(const ChordDef &def) const
	{
		if (has7(def) && has9(def) && has11(def) && has13(def))
		{
			return OctaveMap({
				{com::degrees::I, -1},
				{com::degrees::IV, -1},
				{com::degrees::VII, -1},
				{com::degrees::II, 0},
				{com::degrees::VI, 0},
			});
		}
		if (has7(def) && has9(def) && has11(def))
		{
			return OctaveMap({
				{com::degrees::I, -1},
				{com::degrees::IV, -1},
				{com::degrees::VII, -1},
				{com::degrees::II, 0},
				{com::degrees::V, 0},
			});
		}
		if (has7(def) && has9(def))
		{
			return OctaveMap({
				{com::degrees::I, -1},
				{com::degrees::III, -1},
				{com::degrees::VII, -1},
				{com::degrees::II, 0},
				{com::degrees::V, 0},
			});
		}
		if (has7(def))
		{
			return OctaveMap({
				{com::degrees::I, -1},
				{com::degrees::V, -1},
				{com::degrees::VII, -1},
				{com::degrees::III, 0},
				{com::degrees::V, 0},
				{com::degrees::I, 1},
			});
		}
		return OctaveMap({
			{com::degrees::I, -1},
			{com::degrees::V, -1},
			{com::degrees::I, 0},
			{com::degrees::III, 0},
			{com::degrees::V, 0},
			{com::degrees::I, 1},
		});
	}

	SimpleGuitar::Pitches SimpleGuitar::get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo &timeInfo)
	{

		if (degreeIntervals.size() < 3)
		{
			return Base::get(chord, def, degreeIntervals, timeInfo);
		}
		bool lowerRange = parameters[argumentNames.SimpleGuitarVoicingStrategy.Range].value<com::String>() == "lowerRange";
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		auto octaves = createOctaveMap(def);
		int transpose = 0;
		if (root > com::notes::D && lowerRange)
		{
			transpose = -1;
		}
		for (const auto &degree : degreeIntervals)
		{
			auto interval = def.getDegreeDef(degree);
			if (!interval.valid())
			{
				continue;
			}
			x.pitch = root + (interval.value % 12);
			auto octaveRange = octaves.equal_range(degree.pitch);
			auto octave = octaveRange.first;
			for (; octave != octaveRange.second; ++octave)
			{ // some degrees may be twice (lower I & upper I)
				x.octave = octave->second + transpose;
				result.push_back(x);
			}
		}

		return result;
	}
}