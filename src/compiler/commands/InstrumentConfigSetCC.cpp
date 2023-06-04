#include "InstrumentConfigSetCC.h"
#include <compiler/context/IContext.h>
#include <compiler/Instrument.h>

namespace compiler
{
    void InstrumentConfigSetCC::execute(IContextPtr context)
    {
        if (!this->hasInstrument())
        {
            return;
        }
        int nr = getControllerNr();
        auto value = parameters[argumentNames.SetCC.Value].value<int>();
        AInstrumentDef::CcValue ccValue;
        ccValue.nr = nr;
        ccValue.value = value;
        this->getInstrument()->ccValues.push_back(ccValue);
    }
}