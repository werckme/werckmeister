#ifndef INSTRUMENTCONFIGSETVOICINGSTRATEGY_HPP
#define INSTRUMENTCONFIGSETVOICINGSTRATEGY_HPP

#include "SetVoicingStrategy.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace sheet {
    namespace compiler {
        class InstrumentConfigSetVoicingStrategy : public SetVoicingStrategy, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

#endif