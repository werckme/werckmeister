#include "SetVolume.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetVolume::execute(AContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<int>();
            if (this->hasInstrument()) {
                this->getInstrument()->volume = setTo;
                return;
            }
            context->setVolume(setTo);
        }

    }
}