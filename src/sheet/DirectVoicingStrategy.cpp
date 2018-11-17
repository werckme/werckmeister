#include "DirectVoicingStrategy.h"

namespace sheet {
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals)
	{
		Pitches result;
		auto chordElements = chord.chordElements();
		auto root = std::get<0>(chordElements);
		return degreeIntervals;
	}
}