#include "styleRenderer.h"

namespace sheet {
    namespace compiler {

		void StyleRenderer::switchStyle(IStyleDefServer::ConstStyleValueType current, IStyleDefServer::ConstStyleValueType next)
		{
			if (next == nullptr || current == next) {
				return;
			}
			// set position for new track
			auto chordMeta = ctx_->voiceMetaData();
			for (const auto &track : *next) {
				for (const auto &voice : track.voices)
				{
					setTargetCreateIfNotExists(track, voice);
					auto meta = ctx_->voiceMetaData();
					meta->position = chordMeta->position;
				}
			}
			ctx_->currentStyle(next);
		}

        void StyleRenderer::setTargetCreateIfNotExists(const Track &track, const Voice &voice)
		{
			AContext::TrackId trackId;
			AContext::VoiceId voiceId;
			auto it = ptrIdMap_.find(&track);
			bool trackIsNew = false;
			if (it == ptrIdMap_.end()) {
				trackId = ctx_->createTrack();
				ptrIdMap_[&track] = trackId;
				trackIsNew = true;
			}
			else {
				trackId = it->second;
			}
			it = ptrIdMap_.find(&voice);
			if (it == ptrIdMap_.end()) {
				voiceId = ctx_->createVoice();
				ptrIdMap_[&voice] = voiceId;
			}
			else {
				voiceId = it->second;
			}
			ctx_->setTarget(trackId, voiceId);
			if (trackIsNew) {
				ctx_->processMeta(track.trackInfos, 
					[](const auto &x) { return x.name; }, 
					[](const auto &x) { return x.args; }
				);
			}
		}

        void StyleRenderer::sheetRest(fm::Ticks duration)
		{
			auto styleTracks = ctx_->currentStyle();

			for (const auto &track : *styleTracks)
			{
				for (const auto &voice : track.voices)
				{
					setTargetCreateIfNotExists(track, voice);
					auto meta = ctx_->voiceMetaData();
					ctx_->rest(duration);
				}
			}
		}

        void StyleRenderer::render(fm::Ticks duration)
        {
            auto styleTracks = ctx_->currentStyle();

			for (const auto &track : *styleTracks)
			{
				for (const auto &voice : track.voices)
				{
					if (voice.events.empty()) {
						continue;
					}
					setTargetCreateIfNotExists(track, voice);
					auto meta = ctx_->voiceMetaData();
					fm::Ticks writtenDuration = 0;
					while ((duration - writtenDuration) > AContext::TickTolerance) { // loop until enough events are written
						auto it = voice.events.begin();
						if (meta->idxLastWrittenEvent >= 0) { // continue rendering
							it += meta->idxLastWrittenEvent;
							meta->idxLastWrittenEvent = -1;
							if (it->type == Event::EOB) {
								// happens: | r1 |  ->  | A B |
								// after a half rest the next event would be a new bar
								// its length check would produce a message
								++it;
								meta->barPosition = 0;
							}
						}
						else if (meta->eventOffset > 0) { // skip events (for upbeat)
							it += meta->eventOffset;
						}
						for (; it != voice.events.end(); ++it) // loop voice events
						{
							auto ev = *it;
							auto currentPos = meta->position;
							auto originalDuration = ev.duration;
							if (ev.isTimeConsuming() && meta->remainingTime > 0) {
								if (ev.duration == 0) {
									ev.duration = meta->lastEventDuration;
								}
								ev.duration = ev.duration + meta->remainingTime;
								meta->remainingTime = 0;
							}
							ev.duration = std::min(ev.duration, duration - writtenDuration);
							ctx_->addEvent(ev);
							writtenDuration += meta->position - currentPos;
							if ((duration - writtenDuration) <= AContext::TickTolerance) {
								bool hasRemainings = ev.duration != originalDuration;
								if (hasRemainings) {
									meta->remainingTime = originalDuration - ev.duration;
								}
								meta->idxLastWrittenEvent = it - voice.events.begin() + 1;
								break;
							}
						}
					}
				}
			}
        }
    }
}