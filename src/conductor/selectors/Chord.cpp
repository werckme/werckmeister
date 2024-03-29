#include "Chord.h"

namespace conductor
{
	bool Chord::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.midiEvent;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (!eventInformation || eventInformation->chordRenderInfo.get() == nullptr) {
				continue;
			}
			auto chordInfos = eventInformation->chordRenderInfo;
			const auto eventChordName = chordInfos->chordEvent.stringValue;
			if (eventChordName == arg.name) 
			{
				return true;
			}
			const auto chordQuality = chordInfos->chordEvent.chordDefName();
			if (chordQuality == arg.name) 
			{
				return true;
			}
		}
		return false;
	}
}
