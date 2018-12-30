#include "MidiContext.h"
#include <fm/midi.hpp>
#include <fm/config.hpp>
#include <math.h>

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

			int getAbsoluteVelocity(fm::Expression expression)
			{
				float expr = static_cast<float>(expression);
				return static_cast<int>(::ceil((expr) * 127.0f / 10.0f));
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
			for (const auto &instrument : voiceConfig->instrumentDefs) {
				midi_->tracks().at(track())->events().addNote(
					instrument.channel, 
					absolutePosition, 
					getAbsolutePitch(pitch), 
					getAbsoluteVelocity( voiceConfig->expression ),
					duration);
			}
		}

		void MidiContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			Base::startEvent(pitch, absolutePosition);
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			for (const auto &instrument : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::NoteOn(instrument.channel, 
					absolutePosition, 
					getAbsolutePitch(pitch), 
					getAbsoluteVelocity(voiceConfig->expression)
				);
				midi_->tracks().at(track())->events().add(event);
			}
		}

		
		void MidiContext::stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			Base::stopEvent(pitch, absolutePosition);
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			for (const auto &instrument : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::NoteOff(instrument.channel, 
					absolutePosition, 
					getAbsolutePitch(pitch)
				);
				midi_->tracks().at(track())->events().add(event);
			}
		}

		void MidiContext::addEvent(const fm::midi::Event &ev)
		{
			midi_->tracks().at(track())->events().add(ev);
		}

		void MidiContext::addPitchbendEvent(double value, fm::Ticks absolutePosition) 
		{
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			for (const auto &instrument : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::PitchBend(instrument.channel, absolutePosition, value);
				midi_->tracks().at(track())->events().add(event);
			}
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

		void MidiContext::metaSetTempo(double bpm)
		{
			Base::metaSetTempo(bpm);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto tempoEvent = fm::midi::Event::MetaTempo(bpm);
			tempoEvent.absPosition(meta->position);
			addEvent(tempoEvent);
		}

		void MidiContext::metaSetChannel(int channel)
		{
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			meta->instrumentDefs.at(0).channel = channel;
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
			// send instrument name meta event
			auto trackName = fm::midi::Event::MetaTrack(fm::to_string(uname));
			trackName.absPosition(0);
			addEvent(trackName); 

			// find instrument defs and assign them to the meta data of the voice
			Base::metaSetUname(uname);
			auto range = midiInstrumentDefs_.equal_range(uname);
			if (range.first == midiInstrumentDefs_.end()) {
				return;
			}
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto it = range.first;
			auto end = range.second;
			meta->instrumentDefs.clear();
			for (; it != end; ++it) {
				meta->instrumentDefs.push_back(it->second);
				metaSoundSelect(it->second.cc, it->second.pc);
			}
		}

		void MidiContext::setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def)
		{
			midiInstrumentDefs_.insert({ uname, def });
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