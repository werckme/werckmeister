#pragma once

#include "SetVoicingStrategy.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace compiler
{
    class InstrumentConfigSetVoicingStrategy : public SetVoicingStrategy, public ACanSpecifyInstrument
    {
    public:
        virtual void execute(IContextPtr);
    };
}
