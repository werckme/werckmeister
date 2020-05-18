#include "SetPan.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetPan::execute(AContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<int>();
            if (this->hasInstrument()) {
                this->getInstrument()->pan = setTo;
                return;
            }            
            context->setPan(setTo);
        }
    }
}