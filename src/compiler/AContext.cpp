#include "AContext.h"
#include <exception>
#include <exception>
#include <fm/werckmeister.hpp>
#include <algorithm>

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
			bool renderEvent<Event::Degree>(AContext * ctx, const Event *ev)
			{
				auto chordDef = ctx->currentChordDef();
				auto chord = ctx->currentChord();
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(*chord, *chordDef, ev->pitches);

				for (const auto &pitch : pitches)
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
				ctx->setChord(*chordEv);
				ctx->seek(ev->duration);
				return true;
			}
			template<>
			bool renderEvent<Event::Meta>(AContext * ctx, const Event *ev)
			{
				ctx->setMeta(*ev);
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

		PitchDef AContext::resolvePitch(const PitchDef &pitch) const
		{
			if (pitch.alias.empty()) {
				return pitch;
			}
			const PitchDef *result = styleDefServer()->getAlias(pitch.alias);
			if (result == nullptr) {
				throw std::runtime_error("could not resolve alias: " + fm::to_string(pitch.alias));
			}
			return *result;
		}

		void AContext::addEvent(const PitchDef &rawPitch, fm::Ticks duration, bool tying)
		{
			using namespace fm;
			PitchDef pitch = resolvePitch(rawPitch);
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

		fm::Ticks AContext::barPos() const
		{
			auto meta = voiceMetaData(voice());
			return meta->barPosition;
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

		void AContext::setMeta(const Event &metaEvent)
		{
			if (metaEvent.metaCommand.empty()) {
				throwContextException("invalid meta command ");
			}
		}

		/////////////////////////////////////////////////////////////////////////////
		// Stylerendering
		IStyleDefServer::ConstChordValueType AContext::currentChordDef()
		{
			if (!currentChordDef_) {
				currentChordDef_ = styleDefServer()->getChord(FM_STRING("?"));
			}
			return currentChordDef_;
		}
		IStyleDefServer::ConstStyleValueType AContext::currentStyle()
		{
			if (!currentStyleDef_) {
				currentStyleDef_ = styleDefServer()->getStyle(FM_STRING("?"));
			}
			return currentStyleDef_;
		}
		VoicingStrategyPtr AContext::currentVoicingStrategy()
		{
			if (!currentVoicingStrategy_) {
				currentVoicingStrategy_ = fm::getWerckmeister().getDefaultVoicingStrategy();
			}
			return currentVoicingStrategy_;
		}
		void AContext::setChord(const ChordEvent &chord)
		{
			currentChord_ = chord;
			currentChordDef_ = styleDefServer()->getChord(currentChord_.chordDefName());
			if (currentChordDef_ == nullptr) {
				throw std::runtime_error("chord not found: " + fm::to_string(currentChord_.chordName));
			}
		}
		void AContext::setStyle(const fm::String &styleName)
		{

		}
		void AContext::setTarget(const Track &track, const Voice &voice)
		{
			TrackId trackId;
			VoiceId voiceId;
			auto it = ptrIdMap_.find(&track);
			if (it == ptrIdMap_.end()) {
				trackId = createTrack();
				ptrIdMap_[&track] = trackId;
			}
			else {
				trackId = it->second;
			}
			it = ptrIdMap_.find(&voice);
			if (it == ptrIdMap_.end()) {
				voiceId = createVoice();
				ptrIdMap_[&voice] = voiceId;
			}
			else {
				voiceId = it->second;
			}
			setTarget(trackId, voiceId);
		}
		void AContext::renderStyle(fm::Ticks duration)
		{
			auto chord = currentChord();
			auto styleTracks = currentStyle();
			
			for (const auto &track : *styleTracks)
			{
				for (const auto &voice : track.voices)
				{
					auto meta = voiceMetaData(this->voice());
					fm::Ticks writtenDuration = 0;
					setTarget(track, voice);
					while (writtenDuration < duration) {
						auto it = voice.events.begin();
						for (; it != voice.events.end(); ++it)
						{
							auto ev = *it;
							auto currentPos = voiceMetaData(this->voice())->position;
							ev.duration = std::min(ev.duration, duration - writtenDuration);
							addEvent(ev);
							writtenDuration += voiceMetaData(this->voice())->position - currentPos;
							if (writtenDuration >= duration) {
								break;
							}
						}
					}
				}
			}
		}
	}
}