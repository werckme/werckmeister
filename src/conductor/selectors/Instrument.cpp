#include "Instrument.h"

namespace conductor
{
    bool Instrument::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        for (const auto &argument : arguments)
        {
            const auto *eventInfo = _eventInformationServer->find(*evm.midiEvent);
            if (!eventInfo) 
            {
                continue;
            }
            bool isMatch = eventInfo->instrumentName == argument.name
                || eventInfo->instrumentSectionName == argument.name;
            if (isMatch) 
            {
                return true;
            }
        }
        return false;
    }
}
