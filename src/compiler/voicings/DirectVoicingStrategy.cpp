#include "DirectVoicingStrategy.h"

namespace compiler
{
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::solve(const documentModel::Event &chord, const documentModel::ChordDef &def, const Pitches &absolutePitches, const TimeInfo &)
	{
		return absolutePitches;
	}
}