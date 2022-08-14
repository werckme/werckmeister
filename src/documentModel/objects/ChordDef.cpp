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

	const DegreeDef* ChordDef::findDegreeDef(int degreeValue) const
	{
		DegreeDefs::const_iterator it =
			std::find_if(degreeDefs.begin(), degreeDefs.end(), [degreeValue](const auto& x)
				{ return getDegreeValue(x.degree) == degreeValue; });
		if (it == degreeDefs.end())
		{
			return nullptr;
		}
		return &(*it);
	}

	DegreeDef ChordDef::resolveDegreeDef(const PitchDef &eventPitch) const
	{
		DegreeDef resultDegree;
		auto degreeValueWithBitshiftetFlags = eventPitch.pitch;
		const DegreeDef *degreeDef = findDegreeDef(getDegreeValue(degreeValueWithBitshiftetFlags));
		if (degreeDef == nullptr)
		{
			return DegreeDef::invalid();
		}
		resultDegree = *degreeDef;
		if (resultDegree.isAdjunct && eventPitch.forceDegree == false) {
			return DegreeDef::invalid();
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::Sharp)
		{
			resultDegree.value += 1;
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::DoubleSharp)
		{
			resultDegree.value += 2;
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::TrippleSharp)
		{
			resultDegree.value += 3;
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::Flat)
		{
			resultDegree.value -= 1;
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::DoubleFlat)
		{
			resultDegree.value -= 2;
		}
		if (getFlag(degreeValueWithBitshiftetFlags) == com::degrees::TrippleFlat)
		{
			resultDegree.value -= 3;
		}
		return resultDegree;
	}

	bool has7(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto &degreeDef)
						   { return degreeDef.degree == com::degrees::VII && !degreeDef.isAdjunct; });
	}
	bool has9(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto & degreeDef)
						   { return degreeDef.degree == com::degrees::II && !degreeDef.isAdjunct; });
	}
	bool has11(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto & degreeDef)
						   { return degreeDef.degree == com::degrees::IV && !degreeDef.isAdjunct; });
	}
	bool has13(const ChordDef &def)
	{
		return std::any_of(def.degreeDefs.begin(), def.degreeDefs.end(), [](const auto & degreeDef)
						   { return degreeDef.degree == com::degrees::VI && !degreeDef.isAdjunct; });
	}

}
