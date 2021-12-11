#include "Channel.h"

namespace documentModel
{
    namespace conductor
    {
        bool Channel::isMatch(const ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
        {
            const auto &ev = *evm.noteOn;
            for (const auto &argument : arguments)
            {
                if (ev.channel() == com::Byte(argument.numberValue))
                {
                    return true;
                }
            }
            return false;
        }
    }
}
