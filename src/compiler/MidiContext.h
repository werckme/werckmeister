#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <fm/midi.hpp>

namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
		public:
			typedef AContext Base;
			struct VoiceMetaData : Base::VoiceMetaData {
				int midiChannel = 0;
				int velocity = 90;
			};
			void midi(fm::midi::MidiPtr midi) { midi_ = midi; }
			fm::midi::MidiPtr midi() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) override;
			virtual void addEvent(const fm::midi::Event &ev);
			virtual void metaSetChannel(int channel);
			virtual void metaSoundSelect(int cc, int pc);
			virtual void setMeta(const Event &metaEvent) override;
		protected:
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
		private:
			fm::midi::MidiPtr midi_;
			
        };
    }
}

#endif