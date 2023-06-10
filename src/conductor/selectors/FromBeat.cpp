#include "FromBeat.h"
#include <cmath>

namespace conductor
{
    bool FromBeat::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventInformation = _eventInformationServer->find(ev);
        if(!eventInformation)
        {
            return false;
        }
        auto beat = eventInformation->barPositionQuarters;		
        for (const auto &argument : arguments)
        {
            auto valueToMatch = argument.numberValue - 1;
            if (beat >= valueToMatch)
            {
                return true;
            }
        }
        return false;
    }
}
