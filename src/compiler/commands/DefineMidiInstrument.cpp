#include "DefineMidiInstrument.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        void DefineMidiInstrument::execute(AContext* context)
        {
            auto midiContext = dynamic_cast<MidiContext*>(context);
            if (midiContext == nullptr) {
                FM_THROW(Exception, "failed to get midi context");
            }
            auto name       = parameters[argumentNames.InstrumentDef.WithName].value<fm::String>();
            auto deviceName = parameters[argumentNames.InstrumentDef.OnDevice].value<fm::String>();
            auto ch         = parameters[argumentNames.InstrumentDef.Ch].value<int>();
            auto cc         = parameters[argumentNames.InstrumentDef.Cc].value<int>();
            auto pc         = parameters[argumentNames.InstrumentDef.Pc].value<int>();
            midiContext->setMidiInstrument(name, deviceName, ch, cc, pc);    
        }
    }
}