#include "InstrumentConfigSetVolume.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetVolume::execute(AContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<int>();
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->volume = setTo;
        }
    }
}