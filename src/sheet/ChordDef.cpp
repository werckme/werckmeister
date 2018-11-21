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

}
