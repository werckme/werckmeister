#include "InstrumentConfigSetVoicingStrategy.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void InstrumentConfigSetVoicingStrategy::execute(IContextPtr context)
    {
        if (!this->hasInstrument())
        {
            return;
        }
        bool isAlreadySet = containsVoicingStrategy(getInstrument()->voicingStrategies, voicingStrategy->name());
        if (isAlreadySet)
        {
            return;
        }
        getInstrument()->voicingStrategies.push_back(this->voicingStrategy);
    }
}