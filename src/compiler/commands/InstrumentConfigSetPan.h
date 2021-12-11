#pragma once

#include "SetPan.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace documentModel {
    namespace compiler {
        class InstrumentConfigSetPan : public SetPan, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

