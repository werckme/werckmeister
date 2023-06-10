#include "ToPitch.h"
#include <compiler/context/MidiContext.h>

namespace conductor
{
	bool ToPitch::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		for (const auto &arg : arguments)
		{
			auto pitchdef = _definitionServer->resolvePitch(arg.pitch);
			com::Byte midiPitch = com::Byte(compiler::MidiContext::toMidiPitch(pitchdef));
			if (evm.midiEvent->parameter1() <= midiPitch)
			{
				return true;
			}
		}
		return false;
	}
}
