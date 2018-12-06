#include "ChordDef.h"
#include <algorithm>

namespace sheet {

	const ChordOption * ChordDef::getIntervalBy(fm::Pitch degree) const
	{
		Intervals::const_iterator it = 
			std::find_if(intervals.begin(), intervals.end(), [degree](const auto &x) { return x.degree == degree; });
		if (it == intervals.end()) {
			return nullptr;
		}
		return &(*it);
	}

	bool has7(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::VII; });
	}
	bool has9(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::II; });
	}
	bool has11(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::IV; });
	}
	bool has13(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &chordOption){ return chordOption.degree == fm::degrees::VI; });
	}

}
