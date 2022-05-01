#include "ToBeat.h"
#include <cmath>

namespace conductor
{
	bool ToBeat::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &ev = *evm.noteOn;
        auto eventInformation = _eventInformationServer->find(ev);
        auto beat = eventInformation->barPositionQuarters;		
		for (const auto &argument : arguments)
		{
			auto valueToMatch = argument.numberValue - 1;
			if (beat <= valueToMatch)
			{
				return true;
			}
		}
		return false;
	}
}
