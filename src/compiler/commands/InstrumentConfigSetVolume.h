#ifndef INSTRUMENTCONFIGSETVOLUME_HPP
#define INSTRUMENTCONFIGSETVOLUME_HPP

#include "SetVolume.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace sheet {
    namespace compiler {
        class InstrumentConfigSetVolume : public SetVolume, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(AContext*);
        };
    }
}

#endif