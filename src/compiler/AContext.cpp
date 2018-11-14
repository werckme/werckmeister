#include "AContext.h"
#include <exception>
#include <exception>
#include <fm/werckmeister.hpp>

namespace sheet {

	namespace compiler {

		namespace {
			template<int EventType>
			bool renderEvent(AContext * ctx, const Event *ev)
			{
				return false;
			}

			template<>
			bool renderEvent<Event::Note>(AContext * ctx, const Event *ev)
			{
				for (const auto &pitch : ev->pitches)
				{
					ctx->addEvent(pitch, ev->duration);
				}
				ctx->seek(ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::TiedNote>(AContext * ctx, const Event *ev)
			{
				for (const auto &pitch : ev->pitches)
				{
					ctx->addEvent(pitch, ev->duration, true);
				}
				ctx->seek(ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::EOB>(AContext * ctx, const Event *ev)
			{
				ctx->newBar();
				return true;
			}

			template<>
			bool renderEvent<Event::Rest>(AContext * ctx, const Event *ev)
			{
				ctx->rest(ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::Chord>(AContext * ctx, const Event *ev)
			{
				auto chordEv = static_cast<const ChordEvent*>(ev);
				//ctx->setChord(chordEv->chordName);
				ctx->renderStyle(chordEv->duration);
				return true;
			}
			//////////////////////////////////////////////////
			template <int EventId>
			bool renderEventUnrolled(AContext * ctx, const Event *ev)
			{
				if (ev->type == EventId) {
					return renderEvent<EventId>(ctx, ev);
				}
				return renderEventUnrolled<EventId + 1>(ctx, ev);
			}
			template <>
			bool renderEventUnrolled<Event::NumEvents>(AContext * ctx, const Event *ev)
			{
				return false;
			}

			void _addEvent(AContext * ctx, const Event *ev)
			{
				renderEventUnrolled<0>(ctx, ev);
			}
		}

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

		void AContext::addEvent(const Event &ev)
		{
			_addEvent(this, &ev);
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
		IStyleDefServer::ConstChordValueType AContext::currentChord()
		{
			if (!currentChord_) {
				currentChord_ = styleDefServer()->getChord(FM_STRING("?"));
			}
			return currentChord_;
		}
		IStyleDefServer::ConstStyleValueType AContext::currentStyle()
		{
			if (!currentStyle_) {
				currentStyle_ = styleDefServer()->getStyle(FM_STRING("?"));
			}
			return currentStyle_;
		}
		VoicingStrategyPtr AContext::currentVoicingStrategy()
		{
			if (!currentVoicingStrategy_) {
				currentVoicingStrategy_ = fm::getWerckmeister().getDefaultVoicingStrategy();
			}
			return currentVoicingStrategy_;
		}
		void AContext::setChord(const fm::String &chodrname)
		{

		}
		void AContext::setStyle(const fm::String &styleName)
		{

		}
		void AContext::renderStyle(fm::Ticks duration)
		{
			auto chord = currentChord();
			auto styleTracks = currentStyle();
			for (const auto &track : *styleTracks)
			{
				auto trackId = createTrack();
				for (const auto &voice : track.voices)
				{
					auto voiceId = createVoice();
					setTarget(trackId, voiceId);
					for (const auto &ev : voice.events)
					{

						addEvent(ev);
					}
				}
			}
		}
	}
}