#include "SimpleGuitar.h"
#include <com/tools.h>

namespace compiler
{

	SimpleGuitar::OctaveMap SimpleGuitar::createOctaveMap(const documentModel::ChordDef &def) const
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

	SimpleGuitar::Pitches SimpleGuitar::solve(const documentModel::Event &chord, const documentModel::ChordDef &def, const Pitches &absolutePitches, const TimeInfo &timeInfo)
	{

		if (absolutePitches.size() < 3)
		{
			return Base::solve(chord, def, absolutePitches, timeInfo);
		}
		bool lowerRange = parameters[argumentNames.SimpleGuitarVoicingStrategy.Range].value<com::String>() == "lowerRange";
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		auto octaves = createOctaveMap(def);
		int transpose = 0;
		if (root > com::notes::D && lowerRange)
		{
			transpose = -1;
		}
		for (const auto &pitchDef : absolutePitches)
		{
			auto octaveRange = octaves.equal_range(pitchDef.originalDegree);
			auto octave = octaveRange.first;
			for (; octave != octaveRange.second; ++octave)
			{ // some degrees may be twice (lower I & upper I)
				auto copy = pitchDef;
				copy.octave = octave->second + transpose;
				result.push_back(copy);
			}
		}
		return result;
	}
}