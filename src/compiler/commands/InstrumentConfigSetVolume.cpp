#include "InstrumentConfigSetVolume.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void InstrumentConfigSetVolume::execute(IContextPtr)
    {
        auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
        if (!this->hasInstrument())
        {
            return;
        }
        this->getInstrument()->volume = setTo;
    }
}