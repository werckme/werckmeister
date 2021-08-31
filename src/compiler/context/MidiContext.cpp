#include "MidiContext.h"
#include <fm/midi.hpp>
#include <fm/config.hpp>
#include <math.h>
#include <compiler/error.hpp>
#include <fm/tools.h>
#include <algorithm>
#include <fm/werckmeister.hpp>
#include <compiler/modification/AModification.h>
#include <fm/config/configServer.h>

#define SHEET_MASTER_TRACKNAME "master track"

namespace sheet {
	namespace compiler {

		namespace {
			inline std::shared_ptr<MidiInstrumentDef> getMidiInstrumentDefOrDefault(AInstrumentDefPtr instrument)
			{
				return std::dynamic_pointer_cast<MidiInstrumentDef>(instrument);
			}
			inline std::shared_ptr<MidiInstrumentDef> getMidiInstrumentDef(AInstrumentDefPtr instrument)
			{
				auto midiInstrumentDef = std::dynamic_pointer_cast<MidiInstrumentDef>(instrument);
				if (!midiInstrumentDef) {
					FM_THROW(Exception, "instrument is null or not a midi instrument");
				}
				return midiInstrumentDef;
			}
			inline bool isMidiInstrumentDef(AInstrumentDefPtr instrument)
			{
				return getMidiInstrumentDef(instrument).get() != nullptr;
			}
			const int MidiSchluesselCOffset = 60;
			MidiContext::Base::VoiceId _voiceIds = 0;
			inline void assertMidi(fm::midi::MidiPtr midi) 
			{
				if (!midi) {
					FM_THROW(Exception, "missing midi object");
				}
			}
			inline void assertMidiInstrument(AInstrumentDefPtr instrument)
			{
				if (!isMidiInstrumentDef(instrument)) {
					FM_THROW(Exception, "instrument is null or not a midi instrument");
				}
			}
			inline int getChannel(const MidiContext::TrackMetaData &meta)
			{
				return getMidiInstrumentDef(meta.instrument)->channel;
			}
		}


		int MidiContext::getAbsolutePitch(const PitchDef &pitch)
		{
			auto value =  MidiSchluesselCOffset + pitch.pitch + (pitch.octave * fm::NotesPerOctave);
			if (value < 0 || value > 127) {
				FM_THROW(Exception, "invalid note value, the highest possible note is g'''''");
			}
			return value;
		}

		int MidiContext::toMidiVelocity(double velocity)
		{
			int result = static_cast<int>(::ceil( velocity * 127.0 ));
			return std::min(127, std::max(0, result));
		} 

		void MidiContext::renderPitch(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity, fm::Ticks duration)
		{
			assertMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!voiceConfig || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}
			const auto &instrumentDef = getMidiInstrumentDef(trackMeta->instrument);
			auto event = fm::midi::Event::NoteOn(instrumentDef->channel, 
				absolutePosition, 
				getAbsolutePitch(pitch), 
				toMidiVelocity(velocity)
			);
			addEvent(event);
			event = fm::midi::Event::NoteOff(instrumentDef->channel, 
				absolutePosition + duration, 
				getAbsolutePitch(pitch)
			);
			addEvent(event);
		}

		void MidiContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity)
		{
			Base::startEvent(pitch, absolutePosition, velocity);
			assertMidi(midi_);
			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!voiceConfig || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto& instrumentDef = getMidiInstrumentDef(trackMeta->instrument);
			auto event = fm::midi::Event::NoteOn(instrumentDef->channel, 
				absolutePosition, 
				getAbsolutePitch(pitch), 
				toMidiVelocity(velocity)
			);
			addEvent(event);
		}

		
		void MidiContext::stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			Base::stopEvent(pitch, absolutePosition);
			assertMidi(midi_);
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto& instrumentDef = getMidiInstrumentDef(trackMeta->instrument);
			auto event = fm::midi::Event::NoteOff(instrumentDef->channel, 
				absolutePosition, 
				getAbsolutePitch(pitch)
			);
			addEvent(event);
		}

		void MidiContext::addEvent(const fm::midi::Event &ev)
		{
			auto trackId = track(); // context's current track
			if (trackId == INVALID_TRACK_ID) {
				FM_THROW(Exception, "failed to add an event without related track");
			}
			addEvent(ev, trackId);
		}

		void MidiContext::addEvent(const fm::midi::Event &ev, TrackId trackId)
		{
			_compilerVisitor->visit(this, ev, trackId);

			auto voiceConfig = voiceMetaData<MidiContext::VoiceMetaData>();
			if (voiceConfig)
			{
				fm::Ticks renderRangeBegin = _options->isBeginSet() ? (_options->getBegin() * fm::PPQ) : 0;
				fm::Ticks renderRangeEnd = _options->isEndSet() ? (_options->getEnd() * fm::PPQ) : INT_MAX;
				bool isInRange = voiceConfig->position >= renderRangeBegin && voiceConfig->position < renderRangeEnd;
				bool canBeSkipped = ev.eventType() == fm::midi::NoteOn 
					|| ev.eventType() == fm::midi::NoteOff;
				if (!isInRange && canBeSkipped) {
					return;
				}
				auto evCopy = ev;
				auto absPos = evCopy.absPosition() * voiceConfig->tempoFactor + voiceConfig->positionOffset;
				if (canBeSkipped) {
					absPos -= renderRangeBegin;
				}
				evCopy.absPosition(absPos);
				midi_->tracks().at(trackId)->events().add(evCopy);
				return;
			}
			midi_->tracks().at(trackId)->events().add(ev);
		}

		void MidiContext::renderPitchbend(double value, fm::Ticks absolutePosition) 
		{
			assertMidi(midi_);
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}			
			const auto& instrumentDef = getMidiInstrumentDef(trackMeta->instrument);
			auto event = fm::midi::Event::PitchBend(instrumentDef->channel, absolutePosition, value);
			addEvent(event);
		}

		void MidiContext::addCue(const fm::String &text, fm::Ticks absolutePosition) 
		{
			auto cue = fm::midi::Event::MetaCue(text);
			cue.absPosition(absolutePosition);
			addEvent(cue); 
		}

		MidiContext::Base::TrackId MidiContext::createTrackImpl()
		{
			assertMidi(midi_);
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

		void MidiContext::setTempo(double bpm)
		{
			Base::setTempo(bpm);
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

		void MidiContext::setSignature(int upper, int lower)
		{
			Base::setSignature(upper, lower);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto sigEvent = fm::midi::Event::MetaSignature(upper, lower);
			sigEvent.absPosition(currentPosition());
			addEvent(sigEvent, masterTrackId());
		}

		void MidiContext::selectMidiSound(int cc, int pc)
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

		AInstrumentDefPtr MidiContext::getInstrumentDef(const fm::String &uname)
		{
			auto it = instrumentDefs_.find(uname);
			if (it == instrumentDefs_.end()) {
				return nullptr;
			}
			return it->second;
		}

		AInstrumentDefPtr MidiContext::currentInstrumentDef()
		{
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			return trackMeta->instrument;
		}

		void MidiContext::setVolume(double volume, fm::Ticks relativePosition)
		{
			Base::setVolume(volume);
			auto meta = voiceMetaData<MidiContext::VoiceMetaData>();
			auto trackMeta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta || !trackMeta) {
				FM_THROW(Exception, "meta data = null");
			}				
			auto midiVol = meta->volume * fm::midi::MaxMidiValue / MAX_VOLUME;
			auto channel = getChannel(*trackMeta);
			auto ev = fm::midi::Event::CCVolume(channel, midiVol);
			ev.absPosition(currentPosition() + relativePosition);
			addEvent(ev); 
		}

		void MidiContext::setPan(double val)
		{
			Base::setPan(val);
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

		void MidiContext::setInstrument(const fm::String &uname)
		{
			// send instrumentDef name meta event
			auto trackName = fm::midi::Event::MetaTrack(uname);
			trackName.absPosition(0);
			addEvent(trackName); 
			// find instrumentDef defs and assign them to the meta data of the voice
			Base::setInstrument(uname);
			auto instrumentDef = getInstrumentDef(uname);
			if (instrumentDef == nullptr) {
				FM_THROW(Exception, "instrument " + uname + " not found");
			}
			instrumentDef->setToContext(this);
		}

		void MidiContext::setInstrument(std::shared_ptr<MidiInstrumentDef> instrumentDef)
		{
			auto meta = trackMetaData<MidiContext::TrackMetaData>();
			if (!meta) {
				FM_THROW(Exception, "meta data = null");
			}
			meta->instrument = instrumentDef;
			if (!instrumentDef->deviceName.empty()) {
				addDeviceChangeEvent(instrumentDef->deviceName, 0);
			}
			selectMidiSound(instrumentDef->cc, instrumentDef->pc);
			// volume
			addEvent(__createVolumeEvent(*instrumentDef, currentPosition()));
			// pan
			addEvent(__createPanEvent(*instrumentDef, currentPosition()));
		}

		void MidiContext::setInstrument(std::shared_ptr<InstrumentSectionDef> def)
		{
			// TODO
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

		void MidiContext::addMidiInstrumentDef(const fm::String &uname, std::shared_ptr<MidiInstrumentDef> def)
		{
			if (instrumentDefs_.find(uname) != instrumentDefs_.end()) {
				FM_THROW(Exception, "instrument " + uname + " already defined");
			}
			instrumentDefs_.insert({ uname, def });
			instrumentDefs_[uname]->id = instrumentDefs_.size();
		}

		void MidiContext::defineMidiInstrument(const fm::String &uname, int channel, int cc, int pc)
		{
			defineMidiInstrument(uname, fm::String(), channel, cc, pc);
		}

		void MidiContext::defineMidiInstrument(const fm::String &uname, const fm::String &deviceName, int channel, int cc, int pc)
		{
			auto def = std::make_shared<MidiInstrumentDef>();
			def->uname = uname;
			def->channel = channel;
			def->cc = cc;
			def->pc = pc;
			def->deviceName = deviceName;
			auto& config = fm::getConfigServer();
			if (!config.getDevice(deviceName)) {
				FM_THROW(Exception, "Device '" + deviceName + "' not found");
			}
			addMidiInstrumentDef(uname, def);
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
		IContextPtr MidiContext::createNewContext() const
		{
			auto midiFile 	= std::make_shared<fm::midi::Midi>(fm::PPQ);
			auto midiContext = std::make_shared<sheet::compiler::MidiContext>
			(
				midiFile, 
				definitionsServer_, 
				_compilerVisitor, 
				_logger,
				_options
			);
			return midiContext;
		}
		void MidiContext::clear() 
		{
			Base::clear();
			instrumentDefs_.clear();
		}
	}
}