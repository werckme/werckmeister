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

			int getChannel(const MidiContext::VoiceMetaData &meta)
			{
				return meta.instrumentDefs.at(0).channel;
			}
		}

		void MidiContext::addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration)
		{
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			midi_->tracks().at(track())->events().addNote(getChannel(*voiceConfig), absolutePosition, getAbsolutePitch(pitch), voiceConfig->velocity, duration);
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
			meta->instrumentDefs.at(0).channel = channel - 1;
		}

		void MidiContext::metaSoundSelect(int cc, int pc)
		{
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto ev = fm::midi::Event();
			ev.channel(getChannel(*meta));
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

		const MidiInstrumentDef * MidiContext::getMidiInstrumentDef(const fm::String &uname) const
		{
			auto it = midiInstrumentDefs_.find(uname);
			if (it == midiInstrumentDefs_.end()) {
				return nullptr;
			}
			return &it->second;
		}

		void MidiContext::metaSetUname(const fm::String &uname)
		{
			Base::metaSetUname(uname);
			const auto * instrumentDef = getMidiInstrumentDef(uname);
			if (!instrumentDef) {
				return;
			}
			metaSetChannel(instrumentDef->channel);
			metaSoundSelect(instrumentDef->cc, instrumentDef->pc);
		}

		void MidiContext::setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def)
		{
			midiInstrumentDefs_[uname] = def;
		}

		void MidiContext::metaInstrument(const fm::String &uname, int chanel, int cc, int pc)
		{
			MidiInstrumentDef def;
			def.channel = chanel;
			def.cc = cc;
			def.pc = pc;
			setMidiInstrumentDef(uname, def);
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
			if (metaEvent.metaCommand == SHEET_META__MIDI_INSTRUMENT_DEF) {
				metaInstrument(getArgument<fm::String>(metaEvent, 0), getArgument<int>(metaEvent, 1), getArgument<int>(metaEvent, 2), getArgument<int>(metaEvent, 3));
			}
		}
	}
}