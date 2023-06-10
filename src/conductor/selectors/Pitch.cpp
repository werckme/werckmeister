#include "Pitch.h"
#include <compiler/context/MidiContext.h>

namespace conductor
{
    bool Pitch::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.midiEvent;
        for (const auto &argument : arguments)
        {
            auto pitchdef = _definitionServer->resolvePitch(argument.pitch);
            com::Byte midiPitch = com::Byte(compiler::MidiContext::toMidiPitch(pitchdef));
            if (ev.parameter1() == midiPitch)
            {
                return true;
            }
        }
        return false;
    }
}
