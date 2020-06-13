#ifndef INSTRUMENTCONFIGADDMOD_HPP
#define INSTRUMENTCONFIGADDMOD_HPP

#include "AddMod.h"
#include "ACanSpecifyInstrument.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class InstrumentConfigAddMod : public AddMod, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

#endif