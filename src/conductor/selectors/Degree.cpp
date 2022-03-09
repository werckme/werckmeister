#include "Degree.h"
#include <compiler/context/MidiContext.h>
#include <compiler/voicings/DirectVoicingStrategy.h>

namespace 
{
	compiler::DirectVoicingStrategy voicingStrategy;
}

namespace conductor
{
	bool Degree::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (eventInformation->chordRenderInfo.get() == nullptr) {
				continue;
			}
			auto chordInfos = eventInformation->chordRenderInfo;
			auto absolutePitches = voicingStrategy.get(chordInfos->chordEvent, chordInfos->chordDef, { arg.pitch }, compiler::TimeInfo());
			if (absolutePitches.empty()) 
			{
				continue;
			}
			auto renderedArgumentDegree = absolutePitches.front();
			bool isSamePitchRegardlessOctave = renderedArgumentDegree.pitch % 12 == midiEvent.parameter1() % 12;
			if (isSamePitchRegardlessOctave) 
			{
				return true;
			}
		}
		return false;
	}
}
