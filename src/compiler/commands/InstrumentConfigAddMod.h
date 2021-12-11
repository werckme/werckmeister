#pragma once

#include "AddMod.h"
#include "ACanSpecifyInstrument.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        class InstrumentConfigAddMod : public AddMod, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

