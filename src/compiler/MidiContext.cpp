#include "MidiContext.h"
#include <fm/midi.hpp>
#include <exception>
#include <fm/config.hpp>

namespace sheet {
	namespace compiler {

		namespace {
			MidiContext::Base::VoiceId _voiceIds = 0;
			inline void _checkMidi(fm::midi::MidiPtr midi) 
			{
				if (!midi) {
					throw std::runtime_error("missing midi object");
				}
			}
			inline void _checkTrackId(MidiContext::Base::TrackId trackId)
			{
				if (trackId == MidiContext::Base::INVALID_TRACK_ID) {
					throw std::runtime_error("no track set");
				}
			}
			inline void _checkVoiceId(MidiContext::Base::VoiceId voiceId)
			{
				if (voiceId == MidiContext::Base::INVALID_VOICE_ID) {
					throw std::runtime_error("no voice set");
				}
			}

			int getAbsolutePitch(const PitchDef &pitch)
			{
				return pitch.pitch + (pitch.octave * fm::NotesPerOctave);
			}
		}

		void MidiContext::addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration)
		{
			_checkMidi(midi_);
			_checkTrackId(track());
			_checkVoiceId(voice());
			const auto& voiceConfig = voiceConfigs[voice()];
			midi_->tracks().at(track())->events().addNote(voiceConfig.midiChannel, absolutePosition, getAbsolutePitch(pitch), voiceConfig.velocity, duration);
		}

		MidiContext::Base::TrackId MidiContext::createTrack()
		{
			_checkMidi(midi_);
			auto track = midi_->createTrack();
			midi_->addTrack(track);
			return midi_->tracks().size() - 1;
		}
		MidiContext::Base::VoiceId MidiContext::createVoice()
		{
			auto id = _voiceIds++;
			voiceConfigs[id] = VoiceConfig();
			return id;
		}
	}
}