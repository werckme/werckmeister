#include "Degree.h"
#include <compiler/context/MidiContext.h>
#include <compiler/voicings/DirectVoicingStrategy.h>

namespace 
{
	compiler::VoicingStrategies voicingStrategies(
		{ std::make_shared<compiler::DirectVoicingStrategy>() }
	);
}

namespace conductor
{
	bool Degree::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
	{
		const auto &midiEvent = *evm.midiEvent;
		for (const auto &arg : arguments)
		{
			auto eventInformation = _eventInformationServer->find(midiEvent);
			if (!eventInformation || eventInformation->chordRenderInfo.get() == nullptr) {
				continue;
			}
			auto chordInfos = eventInformation->chordRenderInfo;
			documentModel::Event absolutePitch;
			documentModel::Event degreeEvent;
			degreeEvent.type = documentModel::Event::Degree;
			degreeEvent.pitches = { arg.pitch };
			_definitionsServer->degreeToAbsoluteNote(voicingStrategies, compiler::TimeInfo(), chordInfos->chordEvent, degreeEvent, absolutePitch, true, false);

			auto &absolutePitches = absolutePitch.pitches;
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
