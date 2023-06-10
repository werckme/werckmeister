#include "NotOnBeat.h"
#include <cmath>

namespace
{
    const com::Ticks Tolerance = 0.0001;
}

namespace conductor
{
    bool NotOnBeat::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventInformation = _eventInformationServer->find(ev);
        if (!eventInformation)
        {
            return false;
        }
        auto beat = eventInformation->barPositionQuarters;	        
        for (const auto &argument : arguments)
        {
            auto valueToMatch = argument.numberValue - 1;
            if (::fabs(beat - valueToMatch) <= Tolerance)
            {
                return false;
            }
        }
        return true;
    }
}
