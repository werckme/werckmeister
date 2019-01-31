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
			for (const auto &instrumentDef : voiceConfig->instrumentDefs) {
				midi_->tracks().at(track())->events().addNote(
					instrumentDef.channel, 
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
			for (const auto &instrumentDef : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::NoteOn(instrumentDef.channel, 
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
			for (const auto &instrumentDef : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::NoteOff(instrumentDef.channel, 
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
			for (const auto &instrumentDef : voiceConfig->instrumentDefs) {
				auto event = fm::midi::Event::PitchBend(instrumentDef.channel, absolutePosition, value);
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

		MidiInstrumentDef * MidiContext::getMidiInstrumentDef(const fm::String &uname)
		{
			auto it = midiInstrumentDefs_.find(uname);
			if (it == midiInstrumentDefs_.end()) {
				return nullptr;
			}
			return &it->second;
		}

		AInstrumentDef * MidiContext::getInstrumentDef(const fm::String &uname) 
		{
			return getMidiInstrumentDef(uname);
		}

		void MidiContext::metaSetVolume(int volume)
		{
			Base::metaSetVolume(volume);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto midiVol = meta->volume * fm::midi::MaxMidiValue / MAX_VOLUME;
			auto channel = getChannel(*meta);
			auto ev = fm::midi::Event::CCVolume(channel, midiVol);
			ev.absPosition(meta->position);
			addEvent(ev); 
		}

		void MidiContext::metaSetPan(int val)
		{
			Base::metaSetPan(val);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			auto midiVal = meta->pan * fm::midi::MaxMidiValue / MAX_VOLUME;
			auto channel = getChannel(*meta);
			auto ev = fm::midi::Event::CCPan(channel, midiVal);
			ev.absPosition(meta->position);
			addEvent(ev); 
		}
		namespace {
			fm::midi::Event __createVolumeEvent(MidiInstrumentDef &def, fm::Ticks pos)
			{
				auto midiVol = def.volume * fm::midi::MaxMidiValue / MidiContext::MAX_VOLUME;
				auto ev = fm::midi::Event::CCVolume(def.channel, midiVol);
				ev.absPosition(pos);
				return ev;
			}
			fm::midi::Event __createPanEvent(MidiInstrumentDef &def, fm::Ticks pos)
			{
				auto midiVal = def.pan * fm::midi::MaxMidiValue / MidiContext::MAX_PAN;
				auto ev = fm::midi::Event::CCPan(def.channel, midiVal);
				ev.absPosition(pos);
				return ev;
			}			
		}

		void MidiContext::metaSetInstrumentConfig(const fm::String &uname, const Event::Args &args)
		{
			if (args.size() < 3 || args.size() % 2 == 0) {
				throw std::runtime_error("not enough values for " + fm::to_string(SHEET_META__SET_INSTRUMENT_CONFIG) +  ": " + fm::to_string(uname));
			}
			auto instrumentDef = getMidiInstrumentDef(uname);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>(voice());
			if (instrumentDef == nullptr) {
				throw std::runtime_error("instrumentDef not found: " + fm::to_string(uname));
			}
			for (size_t idx = 1; idx < args.size(); idx+=2) {
				auto propertyName = getArgument<fm::String>(args, idx);
				fm::midi::Event theEvent;
				if (propertyName == SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME) {
					instrumentDef->volume = getArgument<int>(args, idx+1);
					theEvent = __createVolumeEvent(*instrumentDef, meta->position);
				} 
				else if (propertyName == SHEET_META__SET_INSTRUMENT_CONFIG_PAN) {
					instrumentDef->pan = getArgument<int>(args, idx+1);
					theEvent = __createPanEvent(*instrumentDef, meta->position);
				} else {
					throw std::runtime_error("instrumentDef config not found: " + fm::to_string(uname) + ", " + fm::to_string(propertyName));
				}
				addEvent(theEvent); 
			}
		}

		void MidiContext::metaSetInstrument(const fm::String &uname)
		{
			// send instrumentDef name meta event
			auto trackName = fm::midi::Event::MetaTrack(fm::to_string(uname));
			trackName.absPosition(0);
			addEvent(trackName); 

			// find instrumentDef defs and assign them to the meta data of the voice
			Base::metaSetInstrument(uname);
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
				if (!it->second.deviceName.empty()) {
					addDeviceChangeEvent(it->second.deviceName, meta->position);
				}
				metaSoundSelect(it->second.cc, it->second.pc);
				// volume
				addEvent(__createVolumeEvent(it->second, meta->position));
				// pan
				addEvent(__createPanEvent(it->second, meta->position));
			}
		}

		void MidiContext::addDeviceChangeEvent(const fm::String &deviceName, fm::Ticks position)
		{
			if (deviceName.empty()) {
				return;
			}
			fm::midi::CustomMetaData cdata;
			cdata.type = fm::midi::CustomMetaData::SetDevice;
			std::string ndeviceName = fm::to_string(deviceName);
			cdata.data = fm::midi::CustomMetaData::Data(ndeviceName.begin(), ndeviceName.end());
			auto ev = fm::midi::Event::MetaCustom(cdata);
			ev.absPosition(position);
			addEvent(ev);
		}

		void MidiContext::setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def)
		{
			midiInstrumentDefs_.insert({ uname, def });
		}

		void MidiContext::metaInstrument(const fm::String &uname, int channel, int cc, int pc)
		{
			metaInstrument(uname, fm::String(), channel, cc, pc);
		}

		void MidiContext::metaInstrument(const fm::String &uname, const fm::String &deviceName, int channel, int cc, int pc)
		{
			MidiInstrumentDef def;
			def.channel = channel;
			def.cc = cc;
			def.pc = pc;
			def.deviceName = deviceName;
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
				auto name = getArgument<fm::String>(metaEvent, 0);
				size_t numArgs = metaEvent.metaArgs.size();
				if (numArgs == 4) {
					metaInstrument(getArgument<fm::String>(metaEvent, 0), getArgument<int>(metaEvent, 1), getArgument<int>(metaEvent, 2), getArgument<int>(metaEvent, 3));
					return;
				}
				if (numArgs == 5) {
					metaInstrument(getArgument<fm::String>(metaEvent, 0), getArgument<fm::String>(metaEvent, 1),getArgument<int>(metaEvent, 2), getArgument<int>(metaEvent, 3), getArgument<int>(metaEvent, 4));					
					return;
				}
				throw std::runtime_error("invalid number of arguments for instrumentDef: " + fm::to_string(name) );
			}
			if (metaEvent.metaCommand == SHEET_META__SET_INSTRUMENT_CONFIG) {
				metaSetInstrumentConfig(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}	
		}
	}
}