#include "InstrumentConfigSetCC.h"
#include <compiler/context/IContext.h>
#include <compiler/Instrument.h>
#include <com/midi.hpp>
#include <compiler/error.hpp>

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
        if (value < 0 || value > com::midi::MaxMidiValue) 
		{
			FM_THROW(Exception, "invalid midi value: " + std::to_string(value));
		}
		if (nr < 0 || nr > com::midi::MaxMidiValue) 
		{
			FM_THROW(Exception, "invalid controller number: " + std::to_string(nr));
		}
        AInstrumentDef::CcValue ccValue;
        ccValue.nr = nr;
        ccValue.value = value;
        this->getInstrument()->ccValues.push_back(ccValue);
    }
}