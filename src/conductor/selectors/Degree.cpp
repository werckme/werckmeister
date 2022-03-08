#include "Degree.h"
#include <compiler/context/MidiContext.h>

namespace conductor
{
	bool Degree::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (eventInformation->degreeInfos.empty()) {
				continue;
			}
			int i = 0;
		}
		return false;
	}
}
