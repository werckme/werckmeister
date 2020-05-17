#include "MidiContext.h"
#include <fm/midi.hpp>
#include <fm/config.hpp>
#include <math.h>
#include <compiler/error.hpp>
#include <fm/tools.h>
#include <algorithm>
#include <fm/werckmeister.hpp>
#include <compiler/modification/AModification.h>
#include <compiler/commands/ACommand.h>

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

		int MidiContext::toMidiVelocity(double velocity)
		{
			int result = static_cast<int>(::ceil( velocity * 127.0 ));
			return std::min(127, std::max(0, result));
		} 

		void MidiContext::renderPitch(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity, fm::Ticks duration)
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
				toMidiVelocity(velocity)
			);
			addEvent(event, track());
			event = fm::midi::Event::NoteOff(instrumentDef.channel, 
				absolutePosition + duration, 
				getAbsolutePitch(pitch)
			);
			addEvent(event, track());
		}

		void MidiContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity)
		{
			Base::startEvent(pitch, absolutePosition, velocity);
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
				toMidiVelocity(velocity)
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
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			if (voiceConfig) // && voiceConfig->tempoFactor != 1) 
			{
				auto evCopy = ev;
				evCopy.absPosition(evCopy.absPosition() * voiceConfig->tempoFactor + voiceConfig->positionOffset);
				midi_->tracks().at(trackId)->events().add(evCopy);
				return;
			}
			midi_->tracks().at(trackId)->events().add(ev);
		}

		void MidiContext::renderPitchbend(double value, fm::Ticks absolutePosition) 
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
			bool isMasterTempoValue = !meta;
			if (isMasterTempoValue) {
				// set global tempo
				auto tempoEvent = fm::midi::Event::MetaTempo(bpm);
				tempoEvent.absPosition(currentPosition());
				masterTempo(bpm);
				addEvent(tempoEvent, masterTrackId());
				midi_->bpm(bpm);
			} else {
				auto oldFactor = meta->tempoFactor;
				meta->tempoFactor = masterTempo() / bpm;
				// update offset, to fix position after tempo change
				auto fixQuotient = meta->tempoFactor / oldFactor;
				meta->position /= fixQuotient;
				meta->barPosition /= fixQuotient;
			}

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

		AInstrumentDef * MidiContext::currentInstrumentDef()
		{
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			return &trackMeta->instrument;
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
			
			static const std::vector<fm::String> keywords = {
				SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME,
				SHEET_META__SET_INSTRUMENT_CONFIG_PAN,
				SHEET_META__SET_VOICING_STRATEGY,
				SHEET_META__SET_MOD
			}; 			
			if (args.size() < 3 || args.size() % 2 == 0) {
				FM_THROW(Exception, "not enough values for " + fm::String(SHEET_META__SET_INSTRUMENT_CONFIG) +  ": " + uname);
			}
			auto instrumentDef = getMidiInstrumentDef(uname);
			if (instrumentDef == nullptr) {
				FM_THROW(Exception, "instrumentDef not found: " + uname);
			}
			auto argsBegin = args.begin() + 1;
			auto argsEnd = args.end();
			auto argsExceptFirst = Event::Args(argsBegin, argsEnd);
			// #74.2 TODO
			instrumentDef->volume = fm::getArgValueFor<int>(SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME, argsExceptFirst, instrumentDef->volume);
			instrumentDef->pan = fm::getArgValueFor<int>(SHEET_META__SET_INSTRUMENT_CONFIG_PAN, argsExceptFirst, instrumentDef->pan);
			//auto argsMappedByKeyword = mapArgumentsByKeywords(args, keywords);
			// auto argsRange = argsMappedByKeyword.equal_range(SHEET_META__SET_VOICING_STRATEGY);
			// assign voicingStrategies
			// if (argsRange.first != argsRange.second) {
			// 	auto &wm = fm::getWerckmeister();
			// 	auto it = argsRange.first;
			// 	for (; it != argsRange.second; ++it) {
			// 		const auto &args = it->second;
			// 		if (args.empty()) {
			// 			FM_THROW(Exception, fm::String("missing arguments for: ") + SHEET_META__SET_VOICING_STRATEGY);
			// 		}
			// 		instrumentDef->voicingStrategy = wm.getVoicingStrategy(args.front());
			// 		instrumentDef->voicingStrategy->setArguments(args);
			// 	}
			// }
			// mods
			// argsRange = argsMappedByKeyword.equal_range(SHEET_META__SET_MOD);
			// if (argsRange.first != argsRange.second) {
			// 	auto &wm = fm::getWerckmeister();
			// 	auto it = argsRange.first;
			// 	for (; it != argsRange.second; ++it) {
			// 		const auto &args = it->second;
			// 		if (args.empty()) {
			// 			FM_THROW(Exception, fm::String("missing arguments for: ") + SHEET_META__SET_MOD);
			// 		}					
			// 		auto mod = wm.getModification(args.front());
			// 		mod->setArguments(args);
			// 		instrumentDef->modifications.push_back(mod);
			// 	}				
			// }		
			// velocity overrides
			auto assignIfSet = [&argsExceptFirst, instrumentDef, this](const fm::String &expression){
				// #74.2 TODO
				// auto foundValue = fm::getArgumentValue<int>(expression, argsExceptFirst);
				// if (!foundValue.first) {
				// 	return;
				// }
				// if (foundValue.second < 0 || foundValue.second > 100) {
				// 	FM_THROW(Exception, "invalid value for: " + expression);
				// }
				// auto exprValue = getExpression(expression);
				// instrumentDef->velocityOverride[exprValue] = foundValue.second;
			};
			for(const auto &keyValue : expressionMap_) {
				assignIfSet(keyValue.first);
			}
		}

		void MidiContext::setInstrument(const fm::String &uname)
		{
			// send instrumentDef name meta event
			auto trackName = fm::midi::Event::MetaTrack(uname);
			trackName.absPosition(0);
			addEvent(trackName); 
			// find instrumentDef defs and assign them to the meta data of the voice
			Base::setInstrument(uname);
			auto instrumentDef = getMidiInstrumentDef(uname);
			if (instrumentDef == nullptr) {
				FM_THROW(Exception, "instrument " + uname + " not found");
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
			std::string ndeviceName = deviceName;
			cdata.data = fm::midi::CustomMetaData::Data(ndeviceName.begin(), ndeviceName.end());
			auto ev = fm::midi::Event::MetaCustom(cdata);
			ev.absPosition(position);
			addEvent(ev);
		}

		void MidiContext::setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def)
		{
			if (midiInstrumentDefs_.find(uname) != midiInstrumentDefs_.end()) {
				FM_THROW(Exception, "instrument " + uname + " already defined");
			}
			midiInstrumentDefs_.insert({ uname, def });
			midiInstrumentDefs_[uname].id = midiInstrumentDefs_.size();
		}

		void MidiContext::setMidiInstrument(const fm::String &uname, int channel, int cc, int pc)
		{
			setMidiInstrument(uname, fm::String(), channel, cc, pc);
		}

		void MidiContext::setMidiInstrument(const fm::String &uname, const fm::String &deviceName, int channel, int cc, int pc)
		{
			MidiInstrumentDef def;
			def.uname = uname;
			def.channel = channel;
			def.cc = cc;
			def.pc = pc;
			def.deviceName = deviceName;		
			setMidiInstrumentDef(uname, def);
		}

		void MidiContext::processMeta(const fm::String &commandName, const std::vector<sheet::Argument> &args)
		{
			try {

				auto &wm = fm::getWerckmeister();
				auto command = wm.create<ACommand>(commandName);
				if (command) {
					command->setArguments(args);
					command->execute(this);
					return;
				}

				// #74 TODO: deprecated
				if (commandName == SHEET_META__MIDI_CHANNEL) {
					metaSetChannel(fm::getArgumentValue<int>(args, 0));
					return;
				}
				if (commandName == SHEET_META__MIDI_SOUNDSELECT) {
					// #74.2 TODO
					metaSoundSelect(fm::getArgumentValue<int>(args, 0), fm::getArgumentValue<int>(args, 1));
					return;
				}

				if (commandName == SHEET_META__SET_INSTRUMENT_CONFIG) {
					metaSetInstrumentConfig(fm::getArgumentValue<fm::String>(args, 0), args);
					return;
				}
			} catch(const std::exception &ex) {
				FM_THROW(Exception, "failed to process " + commandName
									+"\n" + ex.what());
			}	
			catch(...) {
				FM_THROW(Exception, "failed to process " + commandName);
			}
			Base::processMeta(commandName, args);	
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