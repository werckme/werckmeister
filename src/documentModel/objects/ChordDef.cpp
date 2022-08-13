#include "ChordDef.h"
#include <algorithm>
#include <documentModel/objects/Event.h>

namespace documentModel
{

	int getDegreeValue(int degree)
	{
		return 0xff & (int)degree;
	}

	com::degrees::Flag getFlag(int degree)
	{
		return static_cast<com::degrees::Flag>((degree) >> 8);
	}

	DegreeDef ChordDef::findDegreeDef(const PitchDef &eventPitch) const
	{
		DegreeDef resultDegree;
		auto degreeValue = eventPitch.pitch;
		DegreeDefs::const_iterator it =
			std::find_if(degreeDefs.begin(), degreeDefs.end(), [degreeValue](const auto &x)
						 { return getDegreeValue(x.degree) == getDegreeValue(degreeValue); });
		if (it == degreeDefs.end())
		{
			return DegreeDef::invalid();
		}
		resultDegree = *it;
		if (resultDegree.isAdjunct && eventPitch.forceDegree == false) {
			return DegreeDef::invalid();
		}
		if (getFlag(degreeValue) == com::degrees::Sharp)
		{
			resultDegree.value += 1;
		}
		if (getFlag(degreeValue) == com::degrees::DoubleSharp)
		{
			resultDegree.value += 2;
		}
		if (getFlag(degreeValue) == com::degrees::TrippleSharp)
		{
			resultDegree.value += 3;
		}
		if (getFlag(degreeValue) == com::degrees::Flat)
		{
			resultDegree.value -= 1;
		}
		if (getFlag(degreeValue) == com::degrees::DoubleFlat)
		{
			resultDegree.value -= 2;
		}
		if (getFlag(degreeValue) == com::degrees::TrippleFlat)
		{
			resultDegree.value -= 3;
		}
		return resultDegree;
	}

	bool has7(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto &DegreeDef)
						   { return DegreeDef.degree == com::degrees::VII; });
	}
	bool has9(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto &DegreeDef)
						   { return DegreeDef.degree == com::degrees::II; });
	}
	bool has11(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto &DegreeDef)
						   { return DegreeDef.degree == com::degrees::IV; });
	}
	bool has13(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto &DegreeDef)
						   { return DegreeDef.degree == com::degrees::VI; });
	}

}
