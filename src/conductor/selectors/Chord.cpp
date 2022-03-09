#include "Chord.h"

namespace conductor
{
	bool Chord::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (eventInformation->chordRenderInfo.get() == nullptr) {
				continue;
			}
			auto chordInfos = eventInformation->chordRenderInfo;
			const auto eventChordName = chordInfos->chordEvent.stringValue;
			if (eventChordName == arg.name) 
			{
				return true;
			}
		}
		return false;
	}
}
