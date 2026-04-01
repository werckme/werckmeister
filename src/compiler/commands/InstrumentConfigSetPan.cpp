#include "InstrumentConfigSetPan.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void InstrumentConfigSetPan::execute(IContextPtr)
    {
        if (!this->hasInstrument())
        {
            return;
        }
        auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
        this->getInstrument()->pan = setTo;
    }
}