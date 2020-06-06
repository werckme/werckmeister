#include "InstrumentConfigAddMod.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigAddMod::execute(AContext* context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            this->getInstrument()->modifications.push_back(theModification);
        }
    }
}