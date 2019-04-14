#include "ChordDef.h"
#include <algorithm>

namespace sheet {


	int getDegreeValue(int degree)
	{
		return 0xff & (int)degree;
	}

	fm::degrees::Flag getFlag(int degree)
	{
		return static_cast<fm::degrees::Flag>((degree) >> 8);
	}

	DegreeDef ChordDef::getDegreeDef(fm::Pitch degree) const
	{
		Intervals::const_iterator it = 
			std::find_if(intervals.begin(), intervals.end(), [degree](const auto &x) 
			{ 
				return getDegreeValue(x.degree) == getDegreeValue(degree); 
			});
		if (it == intervals.end()) {
			return DegreeDef::invalid();
		}
		auto res = *it;
		if (getFlag(degree) == fm::degrees::Sharp) {
			res.value += 1;
		}
		if (getFlag(degree) == fm::degrees::Flat) {
			res.value -= 1;
		}
		return res;
	}

	bool has7(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &DegreeDef){ return DegreeDef.degree == fm::degrees::VII; });
	}
	bool has9(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &DegreeDef){ return DegreeDef.degree == fm::degrees::II; });
	}
	bool has11(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &DegreeDef){ return DegreeDef.degree == fm::degrees::IV; });
	}
	bool has13(const ChordDef &def)
	{
		return std::any_of(def.intervals.begin(), def.intervals.end(), [](const auto &DegreeDef){ return DegreeDef.degree == fm::degrees::VI; });
	}

}
