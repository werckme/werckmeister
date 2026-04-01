#include "DirectVoicingStrategy.h"

namespace compiler
{
	DirectVoicingStrategy::Pitches DirectVoicingStrategy::solve(const documentModel::Event &, const documentModel::ChordDef &, const Pitches &absolutePitches, const TimeInfo &)
	{
		return absolutePitches;
	}
}