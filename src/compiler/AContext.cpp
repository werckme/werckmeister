#include "AContext.h"
#include <exception>

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
	
		AContext::TrackId AContext::createTrack()
		{
			auto tid = createTrackImpl();
			return tid;
		}
		AContext::VoiceId AContext::createVoice()
		{
			VoiceId vid = createVoiceImpl();
			auto meta = createVoiceMetaData();
			voiceMetaDataMap_[vid] = meta;
			return vid;
		}

		AContext::VoiceMetaDataPtr AContext::voiceMetaData(VoiceId voiceid) const
		{
			auto it = voiceMetaDataMap_.find(voiceid);
			if (it == voiceMetaDataMap_.end()) {
				throw std::runtime_error("no meta data found for voiceid: " + std::to_string(voiceid));
			}
			return it->second;
		}
	}
}