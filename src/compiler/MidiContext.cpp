#include "MidiContext.h"
#include <fm/midi.hpp>
#include <fm/config.hpp>

namespace sheet {
	namespace compiler {

		namespace {
			const int MidiSchluesselCOffset = 60;
			MidiContext::Base::VoiceId _voiceIds = 0;
			inline void _checkMidi(fm::midi::MidiPtr midi) 
			{
				if (!midi) {
					throw std::runtime_error("missing midi object");
				}
			}

			int getAbsolutePitch(const PitchDef &pitch)
			{
				return MidiSchluesselCOffset + pitch.pitch + (pitch.octave * fm::NotesPerOctave);
			}
		}

		void MidiContext::addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration)
		{
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			midi_->tracks().at(track())->events().addNote(voiceConfig->midiChannel, absolutePosition, getAbsolutePitch(pitch), voiceConfig->velocity, duration);
		}

		MidiContext::Base::TrackId MidiContext::createTrackImpl()
		{
			_checkMidi(midi_);
			auto track = midi_->createTrack();
			midi_->addTrack(track);
			return midi_->tracks().size() - 1;
		}
		MidiContext::Base::VoiceId MidiContext::createVoiceImpl()
		{
			auto id = _voiceIds++;
			return id;
		}

		MidiContext::Base::VoiceMetaDataPtr MidiContext::createVoiceMetaData() 
		{
			return std::make_shared<MidiContext::VoiceMetaData>();
		}
	}
}