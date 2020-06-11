#include "InstrumentConfigAddMod.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigAddMod::execute(IContext* context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->modifications.push_back(theModification);
        }
    }
}