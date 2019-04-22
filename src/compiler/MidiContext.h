#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <fm/midi.hpp>
#include <map>
#include <vector>

namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
		public:
			typedef AContext Base;
			typedef std::unordered_map<fm::String, MidiInstrumentDef> MidiInstrumentDefs;
			typedef std::vector<MidiInstrumentDef> InstrumentDefContainer;
			struct VoiceMetaData : sheet::compiler::VoiceMetaData {};
			struct TrackMetaData : sheet::compiler::TrackMetaData {
				MidiInstrumentDef instrument;
			};
			void midi(fm::midi::MidiPtr midi) { midi_ = midi; }
			fm::midi::MidiPtr midi() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) override;
			virtual void addEvent(const fm::midi::Event &ev, TrackId trackId = INVALID_TRACK_ID);
			virtual void addPitchbendEvent(double value, fm::Ticks absolutePosition) override;
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition) override;
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition) override;
			virtual void metaSetChannel(int channel);
			virtual void metaSoundSelect(int cc, int pc);
			virtual void metaInstrument(const fm::String &uname, int channel, int cc, int pc);
			virtual void metaInstrument(const fm::String &uname, const fm::String &deviceName, int channel, int cc, int pc);
			virtual void metaSetInstrumentConfig(const fm::String &uname, const Event::Args &args);
			virtual void metaSetInstrument(const fm::String &uname) override;
			virtual void metaSetTempo(double bpm) override;
			virtual void processMeta(const fm::String &command, const std::vector<fm::String> &args) override;
			virtual void metaSetVolume(int volume) override;
			virtual void metaSetPan(int val) override;
			virtual void metaSetSignature(int upper, int lower) override;
			/**
			 * sends a custom meta event containing a device name
			 */
			virtual void addDeviceChangeEvent(const fm::String &deviceName, fm::Ticks position);
			virtual AInstrumentDef * getInstrumentDef(const fm::String &uname) override;
			MidiInstrumentDef * getMidiInstrumentDef(const fm::String &uname);
			const MidiInstrumentDefs & midiInstrumentDefs() const { return this->midiInstrumentDefs_; }
		protected:
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
			virtual Base::TrackMetaDataPtr createTrackMetaData() override;
			void setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def);
			InstrumentDefContainer getInstruments() const;
			virtual TrackId createMasterTrack() override;
			static int getAbsolutePitch(const PitchDef &pitch);
			static int getAbsoluteVelocity(fm::Expression expression);
		private:
			MidiInstrumentDefs midiInstrumentDefs_;
			fm::midi::MidiPtr midi_;
			
        };
    }
}

#endif