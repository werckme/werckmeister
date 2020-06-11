#include "InstrumentConfigSetVolume.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetVolume::execute(IContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->volume = setTo;
        }
    }
}