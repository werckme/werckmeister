#include "Instrument.h"

namespace conductor
{
    bool Instrument::isMatch(const ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        for (const auto &argument : arguments)
        {
            if (argument.name == evm.instrumentName)
            {
                return true;
            }
        }
        return false;
    }
}
