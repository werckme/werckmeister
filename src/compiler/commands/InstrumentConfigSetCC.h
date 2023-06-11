#pragma once

#include "SetCC.h"
#include "ACanSpecifyInstrument.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    class InstrumentConfigSetCC : public SetCC, public ACanSpecifyInstrument
    {
    public:
        typedef SetCC Base;
        InstrumentConfigSetCC(ASheetEventRendererPtr renderer) : Base(renderer) {}
        virtual void execute(IContextPtr);
    };
}
