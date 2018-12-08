#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <fm/midi.hpp>
#include <map>
#include <vector>

namespace sheet {
    namespace compiler {
		struct MidiInstrumentDef {
			fm::String uname;
			int channel = 0;
			int cc = 0;
			int pc = 0;
		};
        class MidiContext : public AContext {
		public:
			typedef AContext Base;
			typedef std::unordered_multimap<fm::String, MidiInstrumentDef> MidiInstrumentDefs;
			struct VoiceMetaData : Base::VoiceMetaData {
				typedef std::vector<MidiInstrumentDef> InstrumentDefContainer;
				InstrumentDefContainer instrumentDefs = InstrumentDefContainer(1);
			};
			void midi(fm::midi::MidiPtr midi) { midi_ = midi; }
			fm::midi::MidiPtr midi() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) override;
			virtual void addEvent(const fm::midi::Event &ev);
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition) override;
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition) override;
			virtual void metaSetChannel(int channel);
			virtual void metaSoundSelect(int cc, int pc);
			virtual void metaInstrument(const fm::String &uname, int chanel, int cc, int pc);
			virtual void metaSetUname(const fm::String &uname) override;
			virtual void metaSetTempo(double bpm) override;
			virtual void setMeta(const Event &metaEvent) override;
		protected:
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
			const MidiInstrumentDef * getMidiInstrumentDef(const fm::String &uname) const;
			void setMidiInstrumentDef(const fm::String &uname, const MidiInstrumentDef &def);
		private:
			MidiInstrumentDefs midiInstrumentDefs_;
			fm::midi::MidiPtr midi_;
			
        };
    }
}

#endif