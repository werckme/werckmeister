#ifndef COMPILER_MIDICONTEXT_H
#define COMPILER_MIDICONTEXT_H

#include "AContext.h"
#include "forward.hpp"
#include <unordered_map>

namespace sheet {
    namespace compiler {
        class MidiContext : public AContext {
		public:
			typedef AContext Base;
			void midi(fm::midi::MidiPtr midi) { midi_ = midi; }
			fm::midi::MidiPtr midi() const { return midi_; }
			virtual TrackId createTrack() override;
			virtual VoiceId createVoice() override;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) override;
		private:
			struct VoiceConfig {
				int midiChannel = 0;
				int velocity = 70;
			};
			typedef std::unordered_map<VoiceId, VoiceConfig> VoiceConfigs;
			fm::midi::MidiPtr midi_;
			VoiceConfigs voiceConfigs;
			
        };
    }
}

#endif