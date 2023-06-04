#pragma once

#include "SetCC.h"
#include "ACanSpecifyInstrument.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    class InstrumentConfigSetCC : public SetCC, public ACanSpecifyInstrument
    {
    public:
        virtual void execute(IContextPtr);
    };
}
