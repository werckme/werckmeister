#include "Voice.h"

namespace conductor
{
    bool Voice::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        for (const auto &argument : arguments)
        {
            const auto *eventInfo = _eventInformationServer->find(*evm.midiEvent);
            if (!eventInfo) 
            {
                continue;
            }
            bool isMatch = eventInfo->voiceNumber == (int)argument.numberValue;
            if (isMatch) 
            {
                return true;
            }
        }
        return false;
    }
}
