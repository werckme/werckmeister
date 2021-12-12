#pragma once

#include "SetVolume.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace compiler
{
    class InstrumentConfigSetVolume : public SetVolume, public ACanSpecifyInstrument
    {
    public:
        virtual void execute(IContextPtr);
    };
}
