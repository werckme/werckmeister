#include "Channel.h"

namespace sheet
{
    namespace conductor
    {
        bool Channel::isMatch(const ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
        {
            const auto &ev = *evm.noteOn;
            for (const auto &argument : arguments)
            {
                if (ev.channel() == fm::Byte(argument.numberValue))
                {
                    return true;
                }
            }
            return false;
        }
    }
}
