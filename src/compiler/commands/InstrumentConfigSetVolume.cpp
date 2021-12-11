#include "InstrumentConfigSetVolume.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void InstrumentConfigSetVolume::execute(IContextPtr  context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->volume = setTo;
        }
    }
}