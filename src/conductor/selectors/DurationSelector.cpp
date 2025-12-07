#include "DurationSelector.h"
#include <cmath>

#include <iostream>

namespace
{
	const com::Ticks eps = 0.001;
}
namespace conductor
{
	bool DurationSelector::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		if (evm.noteOff == nullptr)
		{
			return false;
		}
		
		const com::midi::Event &noteOn = *evm.midiEvent;
        const com::midi::Event &noteOff = *evm.noteOff;
		auto duration = (noteOff.absPosition() - noteOn.absPosition()) / com::PPQ;

		for (const auto &argument : arguments)
        {
            auto valueToMatch = argument.numberValue;
			auto diff = std::abs(valueToMatch - duration);
			if (diff <= eps)
			{
				return true;
			}
		}
		return false;
	}
}
