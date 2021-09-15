#include "Pitch.h"
#include <compiler/context/MidiContext.h>

namespace sheet
{
    namespace conductor
    {
        bool Pitch::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto& argument : arguments) {
                auto pitchdef = _definitionServer->resolvePitch(argument.pitch);
                fm::Byte midiPitch = fm::Byte(compiler::MidiContext::toMidiPitch(pitchdef));
                if (ev.parameter1() == midiPitch) {
                    return true;
                }
            }
            return false;
        }
    }
}
