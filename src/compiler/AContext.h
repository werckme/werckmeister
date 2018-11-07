#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>

namespace sheet {
    namespace compiler {
        class AContext {
        public:
			static const fm::Ticks DefaultDuration;
			static const fm::Ticks DefaultBarLength;
			enum { INVALID_TRACK_ID = -1, INVALID_VOICE_ID = -1 };
			typedef int TrackId;
			typedef int VoiceId;
			struct VoiceMetaData {
				typedef std::map<PitchDef, fm::Ticks> WaitForTieBuffer;
				fm::Ticks position = 0;
				fm::Ticks duration = DefaultDuration;
				fm::Ticks barLength = DefaultBarLength;
				fm::Ticks barPosition = 0;
				WaitForTieBuffer waitForTieBuffer;
				virtual ~VoiceMetaData() = default;
				bool pendingTie() const { return !waitForTieBuffer.empty(); }
			};
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			virtual void setTrack(TrackId trackId);
			virtual void setVoice(VoiceId voice);
			TrackId track() const;
			VoiceId voice() const;
			inline void setTarget(TrackId trackId, VoiceId voiceId)
			{
				setTrack(trackId);
				setVoice(voiceId);
			}
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
			/////// actual context stuff
			virtual void addEvent(const PitchDef &pitch, fm::Ticks duration, bool tying = false);
			virtual void seek(fm::Ticks duration);
			virtual void newBar();
			virtual void rest(fm::Ticks duration);
		protected:
			virtual TrackId createTrackImpl() = 0;
			virtual VoiceId createVoiceImpl() = 0;
			virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
			virtual void throwContextException(const std::string &msg);
		private:
			TrackId trackId_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID;
			VoiceMetaDataMap voiceMetaDataMap_;

        };
    }
}

#endif