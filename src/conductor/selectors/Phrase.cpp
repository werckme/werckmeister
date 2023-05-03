#include "Phrase.h"
#include <compiler/context/MidiContext.h>

namespace conductor
{
	bool Phrase::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			const auto &phraseName = eventInformation->phraseName;
			if (phraseName.empty()) {
				continue;
			}
			if (arg.name == phraseName)
			{
				return true;
			}
		}
		return false;
	}
}
