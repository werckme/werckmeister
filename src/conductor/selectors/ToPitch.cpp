#include "ToPitch.h"
#include <compiler/context/MidiContext.h>

namespace documentModel
{
    namespace conductor
    {
        bool ToPitch::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
			for (const auto& arg : arguments)
			{
				auto pitchdef = _definitionServer->resolvePitch(arg.pitch);
				com::Byte midiPitch = com::Byte(compiler::MidiContext::toMidiPitch(pitchdef));
				if (evm.noteOn->parameter1() <= midiPitch) {
					return true;
				}
			}
			return false;
        }
    }
}
