#include "DirectVoicingStrategy.h"

namespace compiler
{
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::solve(const documentModel::Event &chord, const documentModel::ChordDef &def, const Pitches &degreePitches, const TimeInfo &)
	{
		using namespace documentModel;
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		for (const auto &degree : degreePitches)
		{
			auto interval = def.resolveDegreeDef(degree);
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