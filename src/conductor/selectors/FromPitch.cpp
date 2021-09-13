#include "FromPitch.h"
#include <compiler/context/MidiContext.h>
namespace sheet
{
	namespace conductor
	{
		bool FromPitch::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const
		{
			for (const auto& arg : arguments) 
			{
				auto pitchdef = _definitionServer->resolvePitch(arg.pitch);
				fm::Byte midiPitch = fm::Byte(compiler::MidiContext::toMidiPitch(pitchdef));
				if (evm.noteOn->parameter1() >= midiPitch) {
					return true;
				}
			}
			return false;
		}
	}
}
