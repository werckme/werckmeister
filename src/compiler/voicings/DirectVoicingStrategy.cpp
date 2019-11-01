#include "DirectVoicingStrategy.h"

namespace sheet {
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&)
	{
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		for (const auto& degree : degreeIntervals) {
			auto interval = def.getDegreeDef(degree.pitch);
			if (!interval.valid()) {
				continue;
			}
			x.pitch = root + interval.value;
			x.octave = degree.octave;
			result.insert(x);
		}
		return result;
	}
}