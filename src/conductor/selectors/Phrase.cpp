#include "Phrase.h"
#include <compiler/context/MidiContext.h>
#include <algorithm>

namespace conductor
{
	bool Phrase::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (!eventInformation)
			{
				continue;
			}
			const auto &phrases = eventInformation->phrases;
			bool containsPhrase = std::find(phrases.begin(), phrases.end(), arg.name) != phrases.end();
			if (containsPhrase)
			{
				return true;
			}
		}
		return false;
	}
}
