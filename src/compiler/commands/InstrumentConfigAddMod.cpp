#include "InstrumentConfigAddMod.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void InstrumentConfigAddMod::execute(IContextPtr  context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->modifications.push_back(theModification);
        }
    }
}