#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <com/midi.hpp>
#include <map>
#include <vector>
#include <com/config.hpp>
#include <com/ILogger.h>
#include <compiler/ICompilerVisitor.h>

#define SHEET_MASTER_TRACKNAME "master track"

namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
			friend struct MidiInstrumentDef;
			friend struct InstrumentSectionDef;
		private:
			com::midi::MidiPtr midi_;
			ICompilerVisitorPtr _compilerVisitor;
			com::ILoggerPtr _logger;
		public:
			typedef AContext Base;
			typedef std::unordered_map<com::String, AInstrumentDefPtr> InstrumentDefs;
			typedef std::vector<AInstrumentDefPtr> InstrumentDefContainer;
			MidiContext(com::midi::MidiPtr midiFile,
				com::IDefinitionsServerPtr definitionsServer,
				ICompilerVisitorPtr compilerVisitor,
				com::ILoggerPtr logger);

			struct VoiceMetaData : sheet::compiler::VoiceMetaData {
				com::Ticks positionOffset = 0;
			};
			struct TrackMetaData : sheet::compiler::TrackMetaData {
				TrackMetaData();
				AInstrumentDefPtr instrument;
			};
			com::midi::MidiPtr midiFile() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void renderPitch(const PitchDef &pitch, com::Ticks absolutePosition, double velocity, com::Ticks duration) override;
			virtual void addEvent(const com::midi::Event &ev);
			virtual void addEvent(const com::midi::Event &ev, TrackId trackId);
			virtual void renderPitchbend(double value, com::Ticks absolutePosition) override;
			virtual void startEvent(const PitchDef &pitch, com::Ticks absolutePosition, double velocity) override;
			virtual void stopEvent(const PitchDef &pitch, com::Ticks absolutePosition) override;
			virtual void selectMidiSound(int bankMsb, int bankLsb, int pc);
			virtual void defineInstrumentSection(const com::String& uname, std::list<com::String> &sectionInstrumentUNames);
			virtual void defineMidiInstrument(const com::String &uname, int channel, int bankMsb, int bankLsb, int pc);
			virtual void defineMidiInstrument(const com::String &uname, const com::String &deviceName, int channel, int bankMsb, int bankLsb, int pc);
			virtual void setInstrument(const com::String &uname) override;
			virtual void setTempo(double bpm) override;
			virtual void setVolume(double volume, com::Ticks relativePosition = 0) override;
			virtual void setPan(double val) override;
			virtual void addCue(const com::String &text, com::Ticks absolutePosition);
			virtual void setSignature(int upper, int lower) override;
			/**
			 * sends a custom meta event containing a device name
			 */
			virtual void addDeviceChangeEvent(const com::String &deviceName, com::Ticks position);
			virtual void addSetInstrumentEvent(const com::String& instrumentName, com::Ticks position);
			virtual AInstrumentDefPtr getInstrumentDef(const com::String &uname) override;
			virtual AInstrumentDefPtr currentInstrumentDef() override;
			const InstrumentDefs & instrumentDefs() const { return this->instrumentDefs_; }
			virtual IContextPtr createNewContext() const;
			virtual void clear() override;
			static int toMidiPitch(const PitchDef &pitch);
		protected:
			void applyContextInfo(com::midi::Event &ev) const;
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
			virtual Base::TrackMetaDataPtr createTrackMetaData() override;
			void addInstrumentDef(const com::String &uname, AInstrumentDefPtr def);
			virtual TrackId createMasterTrack() override;
			int toMidiVelocity(double velocity);
			void setInstrument(std::shared_ptr<MidiInstrumentDef> def);
			void setInstrument(std::shared_ptr<InstrumentSectionDef> def);
		private:
			InstrumentDefs instrumentDefs_;
        };
		typedef std::shared_ptr<MidiContext> MidiContextPtr;
    }
}

#endif