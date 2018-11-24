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

		void MidiContext::addEvent(const fm::midi::Event &ev)
		{
			midi_->tracks().at(track())->events().add(ev);
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

		void MidiContext::metaSetChannel(int channel)
		{
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			meta->midiChannel = channel;
		}

		void MidiContext::metaSoundSelect(int cc, int pc)
		{
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto ev = fm::midi::Event();
			ev.channel(meta->midiChannel);
			ev.eventType(fm::midi::Controller);
			ev.parameter1(0);
			ev.parameter2(cc);
			ev.absPosition(meta->position);
			addEvent(ev);
			ev.eventType(fm::midi::ProgramChange);
			ev.parameter1(pc);
			ev.parameter2(0);
			addEvent(ev);
		}

		void MidiContext::setMeta(const Event &metaEvent)
		{
			Base::setMeta(metaEvent);

			if (metaEvent.metaCommand == SHEET_META__MIDI_CHANNEL) {
				metaSetChannel(getArgument<int>(metaEvent, 0));
			}
			if (metaEvent.metaCommand == SHEET_META__MIDI_SOUNDSELECT) {
				metaSoundSelect(getArgument<int>(metaEvent, 0), getArgument<int>(metaEvent, 1));
			}
		}
	}
}