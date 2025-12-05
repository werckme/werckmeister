#include "LongerThan.h"
#include <cmath>

namespace conductor
{
	bool LongerThan::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
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
			if (duration >= valueToMatch)
			{
				return true;
			}
		}
		return false;
	}
}
