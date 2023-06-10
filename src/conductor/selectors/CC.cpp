#include "CC.h"
#include <com/midi.hpp>

namespace conductor
{
	bool Cc::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.midiEvent;
		if (midiEvent.eventType() != com::midi::Controller)
		{
			return false;
		}
		auto ccNr = midiEvent.parameter1();
		for (const auto &arg : arguments)
		{
			if (arg.numberValue == ccNr)
			{
				return true;
			}
		}
		return false;
	}
}
