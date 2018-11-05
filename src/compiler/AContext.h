#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "sheet/Event.h"

namespace sheet {
    namespace compiler {
        class AContext {
        public:
			enum { INVALID_TRACK_ID = -1, INVALID_VOICE_ID = -1 };
			typedef int TrackId;
			typedef int VoiceId;
			virtual void track(TrackId trackId);
			virtual void voice(VoiceId voice);
			inline TrackId track() const { return trackId_; }
			inline VoiceId voice() const { return voiceId_; }
			virtual TrackId createTrack() = 0;
			virtual VoiceId createVoice() = 0;
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) = 0;
            virtual ~AContext() = default;
		private:
			TrackId trackId_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID;
        };
    }
}

#endif