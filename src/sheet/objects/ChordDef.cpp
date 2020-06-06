#include "ChordDef.h"
#include <algorithm>
#include <sheet/objects/Event.h>

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
		auto degreeValue = eventPitch.pitch;
		Intervals::const_iterator it = 
			std::find_if(intervals.begin(), intervals.end(), [degreeValue](const auto &x) 
			{ 
				return getDegreeValue(x.degree) == getDegreeValue(degreeValue); 
			});
		if (it == intervals.end()) {
			return DegreeDef::invalid();
		}
		resultDegree = *it;
		if (getFlag(degreeValue) == fm::degrees::Sharp) {
			resultDegree.value += 1;
		}
		if (getFlag(degreeValue) == fm::degrees::DoubleSharp) {
			resultDegree.value += 2;
		}	
		if (getFlag(degreeValue) == fm::degrees::TrippleSharp) {
			resultDegree.value += 3;
		}				
		if (getFlag(degreeValue) == fm::degrees::Flat) {
			resultDegree.value -= 1;
		}
		if (getFlag(degreeValue) == fm::degrees::DoubleFlat) {
			resultDegree.value -= 2;
		}	
		if (getFlag(degreeValue) == fm::degrees::TrippleFlat) {
			resultDegree.value -= 3;
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
