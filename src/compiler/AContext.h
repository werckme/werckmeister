#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>

namespace sheet {
    namespace compiler {
        class AContext {
        public:
			enum { INVALID_TRACK_ID = -1, INVALID_VOICE_ID = -1 };
			typedef int TrackId;
			typedef int VoiceId;
			struct VoiceMetaData {
				virtual ~VoiceMetaData() = default;
			};
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			virtual void track(TrackId trackId);
			virtual void voice(VoiceId voice);
			inline TrackId track() const { return trackId_; }
			inline VoiceId voice() const { return voiceId_; }
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) = 0;
            virtual ~AContext() = default;
			virtual TrackId createTrack();
			virtual VoiceId createVoice();
			VoiceMetaDataPtr voiceMetaData(VoiceId voiceid) const;
			template<typename TVoiceMeta>
			std::shared_ptr<TVoiceMeta> voiceMetaData(VoiceId voiceid) const 
			{
				return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData(voiceid));
			}
		protected:
			virtual TrackId createTrackImpl() = 0;
			virtual VoiceId createVoiceImpl() = 0;
			virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
		private:
			TrackId trackId_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID;
			VoiceMetaDataMap voiceMetaDataMap_;
        };
    }
}

#endif