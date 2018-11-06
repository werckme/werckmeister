#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"


namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
		public:
			typedef AContext Base;
			struct VoiceMetaData : Base::VoiceMetaData {
				int midiChannel = 0;
				int velocity = 70;
			};
			void midi(fm::midi::MidiPtr midi) { midi_ = midi; }
			fm::midi::MidiPtr midi() const { return midi_; }
			virtual TrackId createTrackImpl() override;
			virtual VoiceId createVoiceImpl() override;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) override;
		protected:
			virtual Base::VoiceMetaDataPtr createVoiceMetaData() override;
		private:
			fm::midi::MidiPtr midi_;
			
        };
    }
}

#endif