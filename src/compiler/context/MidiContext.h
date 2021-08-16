#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <fm/midi.hpp>
#include <map>
#include <vector>
#include <fm/config.hpp>
#include <fm/ILogger.h>
#include <compiler/ICompilerVisitor.h>
#include <ICompilerProgramOptions.h>

namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
		private:
			fm::midi::MidiPtr midi_;
			ICompilerVisitorPtr _compilerVisitor;
			fm::ILoggerPtr _logger;
		public:
			typedef AContext Base;
			typedef std::unordered_map<fm::String, MidiInstrumentDef> MidiInstrumentDefs;
			typedef std::vector<MidiInstrumentDef> InstrumentDefContainer;		
			MidiContext(fm::midi::MidiPtr midiFile, 
				fm::IDefinitionsServerPtr definitionsServer, 
				ICompilerVisitorPtr compilerVisitor, 
				fm::ILoggerPtr logger,
				ICompilerProgramOptionsPtr options) 
				: Base(definitionsServer), 
				  midi_(midiFile),
				  _compilerVisitor(compilerVisitor),
				  _logger(logger),
				  _options(options)
				{}

			struct VoiceMetaData : sheet::compiler::VoiceMetaData {
				fm::Ticks positionOffset = 0;
			};
			struct TrackMetaData : sheet::compiler::TrackMetaData {
				MidiInstrumentDef instrument;
			};
			fm::midi::MidiPtr midiFile() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void renderPitch(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity, fm::Ticks duration) override;
			virtual void addEvent(const fm::midi::Event &ev, TrackId trackId = INVALID_TRACK_ID);
			virtual void renderPitchbend(double value, fm::Ticks absolutePosition) override;
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity) override;
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition) override;
			virtual void selectMidiSound(int cc, int pc);
			virtual void setMidiInstrument(const fm::String &uname, int channel, int cc, int pc);
			virtual void setMidiInstrument(const fm::String &uname, const fm::String &deviceName, int channel, int cc, int pc);
			virtual void setInstrument(const fm::String &uname) override;
			virtual void setTempo(double bpm) override;
			virtual void setVolume(double volume, fm::Ticks relativePosition = 0) override;
			virtual void setPan(double val) override;
			virtual void addCue(const fm::String &text, fm::Ticks absolutePosition);
			virtual void setSignature(int upper, int lower) override;
			/**
			 * sends a custom meta event containing a device name
			 */
			virtual void addDeviceChangeEvent(const fm::String &deviceName, fm::Ticks position);
			virtual AInstrumentDef * getInstrumentDef(const fm::String &uname) override;
			MidiInstrumentDef * getMidiInstrumentDef(const fm::String &uname);
			virtual AInstrumentDef * currentInstrumentDef() override;
			const MidiInstrumentDefs & midiInstrumentDefs() const { return this->midiInstrumentDefs_; }
			virtual IContextPtr createNewContext() const;
			virtual void clear() override;
		protected:
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
			virtual Base::TrackMetaDataPtr createTrackMetaData() override;
			void setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def);
			InstrumentDefContainer getInstruments() const;
			virtual TrackId createMasterTrack() override;
			int getAbsolutePitch(const PitchDef &pitch);
			int toMidiVelocity(double velocity);
		private:
			MidiInstrumentDefs midiInstrumentDefs_;
			ICompilerProgramOptionsPtr _options;
        };
		typedef std::shared_ptr<MidiContext> MidiContextPtr;
    }
}

#endif