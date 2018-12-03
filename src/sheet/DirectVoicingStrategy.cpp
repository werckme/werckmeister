#include "DirectVoicingStrategy.h"

namespace sheet {
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&)
	{
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		PitchDef x;
		for (const auto& degree : degreeIntervals) {
			const auto *interval = def.getIntervalBy(degree.pitch);
			if (!interval) {
				continue;
			}
			x.pitch = (root-1) + interval->value;
			x.octave = degree.octave;
			result.insert(x);
		}
		return result;
	}
}