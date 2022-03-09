#include "Chord.h"


namespace conductor
{
	bool Chord::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.noteOn;
		for (const auto &arg : arguments)
		{
			// auto eventInformation = _eventInformationServer->find(midiEvent);
			// if (eventInformation->chordRenderInfo.get() == nullptr) {
			// 	continue;
			// }
			// const auto& chordInfos = *eventInformation->chordRenderInfo;
			// auto absolutePitches = voicingStrategy.get(chordInfos.chordEvent, chordInfos.chordDef, { arg.pitch }, compiler::TimeInfo());
			// if (absolutePitches.empty()) 
			// {
			// 	continue;
			// }
			// auto renderedArgumentDegree = absolutePitches.front();
			// int midiPitchOfRendredDegree = compiler::MidiContext::toMidiPitch(renderedArgumentDegree);
			// bool isSamePitchRegardlessOctave = midiPitchOfRendredDegree == (int)midiEvent.parameter1();
			// if (isSamePitchRegardlessOctave) 
			// {
			// 	return true;
			// }
		}
		return false;
	}
}
