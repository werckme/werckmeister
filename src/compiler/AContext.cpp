#include "AContext.h"
#include <exception>
#include <exception>


namespace sheet {
	namespace compiler {

		using namespace fm;
		const Ticks AContext::DefaultDuration = 1.0_N4;
		const Ticks AContext::DefaultBarLength = 4 * 1.0_N4;

		AContext::IStyleDefServerPtr AContext::styleDefServer() const
		{
			if (!styleDefServer_) {
				throw std::runtime_error("no styledef server set");
			}
			return styleDefServer_;
		}
		void AContext::styleDefServer(IStyleDefServerPtr server)
		{
			styleDefServer_ = server;
		}

		AContext::TrackId AContext::track() const
		{
			if (trackId_ == INVALID_TRACK_ID) {
				throw std::runtime_error("no track set");
			}
			return trackId_;
		}
		AContext::VoiceId AContext::voice() const
		{
			if (voiceId_ == INVALID_VOICE_ID) {
				throw std::runtime_error("no voice set");
			}
			return voiceId_;
		}

		void AContext::setTrack(TrackId trackId)
		{
			this->trackId_ = trackId;
		}

		void AContext::setVoice(VoiceId voice)
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

		void AContext::throwContextException(const std::string &msg)
		{
			auto meta = voiceMetaData(voice());
			throw std::runtime_error(msg + " at voice " + std::to_string(voice()) + " bar: " + std::to_string(meta->position / meta->barLength));
		}

		void AContext::addEvent(const PitchDef &pitch, fm::Ticks duration, bool tying)
		{
			using namespace fm;
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			if (tying) {
				meta->waitForTieBuffer.insert({ pitch, meta->duration });
			}
			else if (meta->pendingTie()) {
				auto it = meta->waitForTieBuffer.find(pitch);
				if (it == meta->waitForTieBuffer.end()) {
					throwContextException("note tie error");
				}
				auto firstDuration = it->second;
				addEvent(pitch, meta->position - firstDuration, firstDuration + meta->duration);
				meta->waitForTieBuffer.erase(it);
				return;
			}
			else {
				addEvent(pitch, meta->position, meta->duration);
			}
		}

		void AContext::seek(fm::Ticks duration)
		{
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			meta->position += meta->duration;
			meta->barPosition += meta->duration;
		}

		void AContext::newBar()
		{
			auto meta = voiceMetaData(voice());
			if (meta->barPosition != meta->barLength) {
				throwContextException("bar check error");
			}
			meta->barPosition = 0;
		}

		void AContext::rest(fm::Ticks duration)
		{
			seek(duration);
		}

		/////////////////////////////////////////////////////////////////////////////
		// Stylerendering
		ChordDef::Intervals* AContext::currentChord()
		{
			return currentChord_;
		}
		StyleDef* AContext::currentStyle()
		{
			return currentStyle_;
		}
		void AContext::setChord(const fm::String &chodrname)
		{

		}
		void AContext::setStyle(const fm::String &styleName)
		{

		}
		void AContext::renderStyle(fm::Ticks duration)
		{
			seek(duration);
		}
	}
}