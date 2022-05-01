#include "FromBar.h"

namespace conductor
{
    bool FromBar::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventInformation = _eventInformationServer->find(ev);
        auto bar = eventInformation->barNumber;		
        for (const auto &argument : arguments)
        {
            int valueToMatch = int(argument.numberValue - 1);
            if (bar >= valueToMatch)
            {
                return true;
            }
        }
        return false;
    }
}
