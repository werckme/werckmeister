#include "MidiContext.h"
#include <fm/midi.hpp>
#include <fm/config.hpp>
#include <math.h>
#include "error.hpp"
#include "sheet/tools.h"

#define SHEET_MASTER_TRACKNAME "master track"

namespace sheet {
	namespace compiler {

		namespace {
			const int MidiSchluesselCOffset = 60;
			MidiContext::Base::VoiceId _voiceIds = 0;
			inline void _checkMidi(fm::midi::MidiPtr midi) 
			{
				if (!midi) {
					FM_THROW(Exception, "missing midi object");
				}
			}

			int getChannel(const MidiContext::TrackMetaData &meta)
			{
				return meta.instrument.channel;
			}
		}


		int MidiContext::getAbsolutePitch(const PitchDef &pitch)
		{
			return MidiSchluesselCOffset + pitch.pitch + (pitch.octave * fm::NotesPerOctave);
		}

		int MidiContext::getAbsoluteVelocity(fm::Expression expression)
		{
			float expr = static_cast<float>(expression);
			return static_cast<int>(::ceil((expr) * 127.0f / 10.0f));
		}

		void MidiContext::addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration)
		{
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!voiceConfig || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}
			const auto &instrumentDef = trackMeta->instrument;
			auto event = fm::midi::Event::NoteOn(instrumentDef.channel, 
				absolutePosition, 
				getAbsolutePitch(pitch), 
				getAbsoluteVelocity(voiceConfig->expression)
			);
			addEvent(event, track());
			event = fm::midi::Event::NoteOff(instrumentDef.channel, 
				absolutePosition + duration, 
				getAbsolutePitch(pitch)
			);
			addEvent(event, track());
		}

		void MidiContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			Base::startEvent(pitch, absolutePosition);
			_checkMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!voiceConfig || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto &instrumentDef = trackMeta->instrument;
			auto event = fm::midi::Event::NoteOn(instrumentDef.channel, 
				absolutePosition, 
				getAbsolutePitch(pitch), 
				getAbsoluteVelocity(voiceConfig->expression)
			);
			addEvent(event, track());
		}

		
		void MidiContext::stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			Base::stopEvent(pitch, absolutePosition);
			_checkMidi(midi_);
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto &instrumentDef = trackMeta->instrument;
			auto event = fm::midi::Event::NoteOff(instrumentDef.channel, 
				absolutePosition, 
				getAbsolutePitch(pitch)
			);
			addEvent(event, track());
		}

		void MidiContext::addEvent(const fm::midi::Event &ev, TrackId trackId)
		{
			if (trackId == INVALID_TRACK_ID) {
				trackId = track(); // context's current track
			}
			if (trackId == INVALID_TRACK_ID) {
				FM_THROW(Exception, "failed to add an event without related track");
			}
			midi_->tracks().at(trackId)->events().add(ev);
		}

		void MidiContext::addPitchbendEvent(double value, fm::Ticks absolutePosition) 
		{
			_checkMidi(midi_);
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto &instrumentDef = trackMeta->instrument;
			auto event = fm::midi::Event::PitchBend(instrumentDef.channel, absolutePosition, value);
			addEvent(event, track());
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

		MidiContext::Base::TrackMetaDataPtr MidiContext::createTrackMetaData() 
		{
			return std::make_shared<MidiContext::TrackMetaData>();
		}

		void MidiContext::metaSetTempo(double bpm)
		{
			Base::metaSetTempo(bpm);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto tempoEvent = fm::midi::Event::MetaTempo(bpm);
			tempoEvent.absPosition(currentPosition());
			addEvent(tempoEvent, masterTrackId());
		}

		void MidiContext::metaSetSignature(int upper, int lower)
		{
			Base::metaSetSignature(upper, lower);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto sigEvent = fm::midi::Event::MetaSignature(upper, lower);
			sigEvent.absPosition(currentPosition());
			addEvent(sigEvent, masterTrackId());
		}

		void MidiContext::metaSetChannel(int channel)
		{
			auto meta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta) {
				FM_THROW(Exception, "meta data = null");
			}			
			meta->instrument.channel = channel;
		}

		void MidiContext::metaSoundSelect(int cc, int pc)
		{
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}				
			auto ev = fm::midi::Event();
			ev.channel(getChannel(*trackMeta));
			ev.eventType(fm::midi::Controller);
			ev.parameter1(0);
			ev.parameter2(cc);
			ev.absPosition(currentPosition());
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
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}				
			auto midiVol = meta->volume * fm::midi::MaxMidiValue / MAX_VOLUME;
			auto channel = getChannel(*trackMeta);
			auto ev = fm::midi::Event::CCVolume(channel, midiVol);
			ev.absPosition(currentPosition());
			addEvent(ev); 
		}

		void MidiContext::metaSetPan(int val)
		{
			Base::metaSetPan(val);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}				
			auto midiVal = meta->pan * fm::midi::MaxMidiValue / MAX_VOLUME;
			auto channel = getChannel(*trackMeta);
			auto ev = fm::midi::Event::CCPan(channel, midiVal);
			ev.absPosition(currentPosition());
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
				FM_THROW(Exception, "not enough values for " + fm::to_string(SHEET_META__SET_INSTRUMENT_CONFIG) +  ": " + fm::to_string(uname));
			}
			auto instrumentDef = getMidiInstrumentDef(uname);
			if (instrumentDef == nullptr) {
				FM_THROW(Exception, "instrumentDef not found: " + fm::to_string(uname));
			}
			for (std::size_t idx = 1; idx < args.size(); idx+=2) {
				auto propertyName = getArgument<fm::String>(args, idx);
				if (propertyName == SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME) {
					instrumentDef->volume = getArgument<int>(args, idx+1);
				} 
				else if (propertyName == SHEET_META__SET_INSTRUMENT_CONFIG_PAN) {
					instrumentDef->pan = getArgument<int>(args, idx+1);
				} else {
					FM_THROW(Exception, "instrumentDef config not found: " + fm::to_string(uname) + ", " + fm::to_string(propertyName));
				}
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
			auto instrumentDef = getMidiInstrumentDef(uname);
			if (instrumentDef == nullptr) {
				FM_THROW(Exception, "instrument " + fm::to_string(uname) + " not found");
			}
			auto meta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta) {
				FM_THROW(Exception, "meta data = null");
			}				
			meta->instrument = *instrumentDef;
			if (!instrumentDef->deviceName.empty()) {
				addDeviceChangeEvent(instrumentDef->deviceName, 0);
			}
			metaSoundSelect(instrumentDef->cc, instrumentDef->pc);
			// volume
			addEvent(__createVolumeEvent(*instrumentDef, currentPosition()));
			// pan
			addEvent(__createPanEvent(*instrumentDef, currentPosition()));
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
			if (midiInstrumentDefs_.find(uname) != midiInstrumentDefs_.end()) {
				FM_THROW(Exception, "instrument " + fm::to_string(uname) + " already defined");
			}
			midiInstrumentDefs_.insert({ uname, def });
			midiInstrumentDefs_[uname].id = midiInstrumentDefs_.size();
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

		void MidiContext::processMeta(const fm::String &command, const std::vector<fm::String> &args)
		{
			try {
				if (command == SHEET_META__MIDI_CHANNEL) {
					metaSetChannel(getArgument<int>(args, 0));
					return;
				}
				if (command == SHEET_META__MIDI_SOUNDSELECT) {
					metaSoundSelect(getArgument<int>(args, 0), getArgument<int>(args, 1));
					return;
				}
				if (command == SHEET_META__MIDI_INSTRUMENT_DEF) {
					auto name = getArgument<fm::String>(args, 0);
					std::size_t numArgs = args.size();
					if (numArgs == 4) {
						metaInstrument(getArgument<fm::String>(args, 0), getArgument<int>(args, 1), getArgument<int>(args, 2), getArgument<int>(args, 3));
						return;
					}
					if (numArgs == 5) {
						metaInstrument(getArgument<fm::String>(args, 0), getArgument<fm::String>(args, 1),getArgument<int>(args, 2), getArgument<int>(args, 3), getArgument<int>(args, 4));					
						return;
					}
					FM_THROW(Exception, "invalid number of arguments for instrumentDef: " + fm::to_string(name) );
				}
				if (command == SHEET_META__SET_INSTRUMENT_CONFIG) {
					metaSetInstrumentConfig(getArgument<fm::String>(args, 0), args);
					return;
				}
			} catch(const std::exception &ex) {
				FM_THROW(Exception, "failed to process " + fm::to_string(command)
									+"\n" + ex.what());
			}	
			catch(...) {
				FM_THROW(Exception, "failed to process " + fm::to_string(command));
			}
			Base::processMeta(command, args);	
		}
		AContext::TrackId MidiContext::createMasterTrack()
		{
			auto trackId = Base::createMasterTrack();
			// send name meta event
			auto trackName = fm::midi::Event::MetaTrack(SHEET_MASTER_TRACKNAME);
			trackName.absPosition(0);
			addEvent(trackName, trackId); 
			return trackId;
		}
	}
}