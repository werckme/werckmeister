#include "InstrumentConfigSetPan.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetPan::execute(IContextPtr  context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
            this->getInstrument()->pan = setTo; 
        }
    }
}