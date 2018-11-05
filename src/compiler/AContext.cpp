#include "AContext.h"

namespace sheet {
	namespace compiler {
	
		void AContext::track(TrackId trackId)
		{
			this->trackId_ = trackId;
		}

		void AContext::voice(VoiceId voice)
		{
			this->voiceId_ = voice;
		}
	
	}
}