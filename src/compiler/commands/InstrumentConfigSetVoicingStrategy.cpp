#include "InstrumentConfigSetVoicingStrategy.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetVoicingStrategy::execute(AContext* context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            getInstrument()->voicingStrategy = this->_voicingStrategy;
        }
    }
}