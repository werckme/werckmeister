#include "InstrumentConfigAddMod.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void InstrumentConfigAddMod::execute(IContextPtr context)
    {
        if (!this->hasInstrument())
        {
            return;
        }
        com::String modName = parameters[argumentNames.AddMod.Use].value<com::String>();
        auto meta = context->voiceMetaData();
        auto theModification = loadMod(context, modName);
        this->getInstrument()->modifications.push_back(theModification);
    }
}