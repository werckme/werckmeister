#include "InstrumentConfigSetVoicingStrategy.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigSetVoicingStrategy::execute(IContextPtr  context)
        {
            if (!this->hasInstrument()) {
                return;
            }
            bool isAlreadySet = getInstrument()->voicingStrategy && getInstrument()->voicingStrategy->name() == voicingStrategy->name();
            if (isAlreadySet) {
                return;
            }
            getInstrument()->voicingStrategy = this->voicingStrategy;
        }
    }
}