#include "SelectMidiSound.h"
#include <compiler/context/MidiContext.h>


namespace sheet {
    namespace compiler {
        void SelectMidiSound::execute(IContextPtr  context)
        {
            auto *midiContext = getMidiContext(context);
            auto pc         = parameters[argumentNames.SelectMidiSound.PC].value<int>();
            auto cc         = parameters[argumentNames.SelectMidiSound.PC].value<int>();
            midiContext->selectMidiSound(cc, pc);
        }
    }
}