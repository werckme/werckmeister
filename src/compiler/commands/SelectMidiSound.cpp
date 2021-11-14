#include "SelectMidiSound.h"
#include <compiler/context/MidiContext.h>


namespace sheet {
    namespace compiler {
        void SelectMidiSound::execute(IContextPtr  context)
        {
            auto *midiContext = getMidiContext(context);
            auto pc         = parameters[argumentNames.SelectMidiSound.PC].value<int>();
            auto bankLsb    = parameters[argumentNames.SelectMidiSound.BankLsb].value<int>();
            auto bankMsb    = parameters[argumentNames.SelectMidiSound.BankMsb].value<int>();
            if (bankMsb < 0)
            {
                bankMsb = parameters[argumentNames.SelectMidiSound.CC].value<int>();
            }
            midiContext->selectMidiSound(bankMsb, bankLsb, pc);
        }
    }
}