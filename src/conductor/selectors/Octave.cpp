#include "Octave.h"
#include <compiler/context/MidiContext.h>

namespace conductor
{
	bool Octave::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		for (const auto &arg : arguments)
		{
			int midiEventOctave = (int)evm.noteOn->parameter1() / 12;
			if (midiEventOctave == arg.numberValue)
			{
				return true;
			}
		}
		return false;
	}
}
