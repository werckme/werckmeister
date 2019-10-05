#include "ChordDef.h"
#include <algorithm>
#include <sheet/Event.h>

namespace sheet {


	int getDegreeValue(int degree)
	{
		return 0xff & (int)degree;
	}

	fm::degrees::Flag getFlag(int degree)
	{
		return static_cast<fm::degrees::Flag>((degree) >> 8);
	}

	DegreeDef ChordDef::getDegreeDef(const PitchDef &eventPitch) const
	{
		DegreeDef resultDegree;
		auto rawEventDegree = eventPitch.pitch;
		if (eventPitch.forceDegree) 
		{
			resultDegree.value = getDegreeValue(rawEventDegree);
		} 
		else 
		{
			Intervals::const_iterator it = 
				std::find_if(intervals.begin(), intervals.end(), [rawEventDegree](const auto &x) 
				{ 
					return getDegreeValue(x.degree) == getDegreeValue(rawEventDegree); 
				});
			if (it == intervals.end()) {
				return DegreeDef::invalid();
			}
			resultDegree = *it;
		}
		
		if (getFlag(rawEventDegree) == fm::degrees::Sharp) {
			resultDegree.value += 1;
		}
		if (getFlag(rawEventDegree) == fm::degrees::Flat) {
			resultDegree.value -= 1;
		}
		return resultDegree;
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
