#ifndef INSTRUMENTCONFIGSETVOLUME_HPP
#define INSTRUMENTCONFIGSETVOLUME_HPP

#include "SetVolume.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace documentModel {
    namespace compiler {
        class InstrumentConfigSetVolume : public SetVolume, public ACanSpecifyInstrument
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

#endif