#include "InstrumentConfigSetVoicingStrategy.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetVoicingStrategy::execute(AContext* context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            bool isAlreadySet = getInstrument()->voicingStrategy && getInstrument()->voicingStrategy->name() == voicingStrategy->name();
            getInstrument()->voicingStrategy = this->voicingStrategy;
        }
    }
}