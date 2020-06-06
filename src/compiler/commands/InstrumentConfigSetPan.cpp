#include "InstrumentConfigSetPan.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetPan::execute(AContext* context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<int>();
            this->getInstrument()->pan = setTo; 
        }
    }
}