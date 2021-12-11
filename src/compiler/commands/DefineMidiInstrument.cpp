#include "DefineMidiInstrument.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>

namespace documentModel {
    namespace compiler {
        void DefineMidiInstrument::execute(IContextPtr  context)
        {
            auto midiContext = getMidiContext(context);
            auto name       = parameters[argumentNames.InstrumentDef.WithName].value<com::String>();
            auto deviceName = parameters[argumentNames.InstrumentDef.OnDevice].value<com::String>();
            auto ch         = parameters[argumentNames.InstrumentDef.Ch].value<int>();
            auto bankMsb    = parameters[argumentNames.InstrumentDef.BankMsb].value<int>();
            auto bankLsb    = parameters[argumentNames.InstrumentDef.BankLsb].value<int>();
            auto pc         = parameters[argumentNames.InstrumentDef.Pc].value<int>();
            if (bankMsb < 0) 
            {
                bankMsb = parameters[argumentNames.InstrumentDef.Cc].value<int>();
            }
            midiContext->defineMidiInstrument(name, deviceName, ch, bankMsb, bankLsb, pc);    
        }
    }
}