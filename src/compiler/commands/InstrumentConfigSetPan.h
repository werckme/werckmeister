#ifndef INSTRUMENTCONFIGSETPAN_HPP
#define INSTRUMENTCONFIGSETPAN_HPP

#include "SetPan.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace sheet {
    namespace compiler {
        class InstrumentConfigSetPan : public SetPan, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(AContext*);
        };
    }
}

#endif