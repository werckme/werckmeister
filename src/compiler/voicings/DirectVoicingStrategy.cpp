#include "DirectVoicingStrategy.h"

namespace compiler
{
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::get(const documentModel::Event &chord, const documentModel::ChordDef &def, const Degrees &degreeIntervals, const TimeInfo &)
	{
		using namespace documentModel;
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		for (const auto &degree : degreeIntervals)
		{
			auto interval = def.getDegreeDef(degree.pitch);
			if (!interval.valid())
			{
				continue;
			}
			x.pitch = root + interval.value;
			x.octave = degree.octave;
			result.push_back(x);
		}
		return result;
	}
}