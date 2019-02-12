#include "styleRenderer.h"
#include "error.hpp"
#include <sheet/tools.h>

namespace sheet {
    namespace compiler {

		void StyleRenderer::switchStyle(const IStyleDefServer::Style &current, const IStyleDefServer::Style &next)
		{
			if (next.empty() || current == next) {
				return;
			}
			// set position for new track
			auto chordMeta = ctx_->voiceMetaData();
			for (auto track : next.tracks) {
				for (const auto &voice : track->voices)
				{
					setTargetCreateIfNotExists(*track, voice);
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
			const auto &styleTracks = ctx_->currentStyle().tracks;

			for (const auto &track : styleTracks)
			{
				for (const auto &voice : track->voices)
				{
					setTargetCreateIfNotExists(*track, voice);
					auto meta = ctx_->voiceMetaData();
					ctx_->rest(duration);
				}
			}
		}

		void StyleRenderer::remberPosition(const Voice &voice, 
			const Event &ev, 
			Voice::Events::const_iterator it,
			fm::Ticks originalEventDuration)
		{
			auto meta = ctx_->voiceMetaData();
			bool hasRemainings = ev.duration != originalEventDuration;
			if (hasRemainings) {
				meta->remainingTime = originalEventDuration - ev.duration;
			}
			meta->idxLastWrittenEvent = it - voice.events.begin() + 1;
		}

		Voice::Events::const_iterator StyleRenderer::skipEvents(Voice::Events::const_iterator it, Voice::Events::const_iterator end, int n)
		{
			it += n;
			if (it->type == Event::EOB) {
				// happens: | r1 |  ->  | A B |
				// after a half rest the next event would be a new bar
				// its length check would produce a message
				auto meta = ctx_->voiceMetaData();
				++it;
				meta->barPosition = 0;
			}
			return it;
		}

		Voice::Events::const_iterator StyleRenderer::continueOnRemeberedPosition(const Voice &voice)
		{
			auto it = voice.events.begin();
			auto meta = ctx_->voiceMetaData();
			it = skipEvents(it, voice.events.end(), meta->idxLastWrittenEvent);
			meta->idxLastWrittenEvent = -1;
			return it;
		}

		fm::Ticks StyleRenderer::renderVoice(const Voice &voice, 
			Voice::Events::const_iterator it, 
			fm::Ticks duration,
			fm::Ticks written)
		{
			auto meta = ctx_->voiceMetaData();
			for (; it < voice.events.end(); ++it) // loop voice events
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
				ev.duration = std::min(ev.duration, duration - written);
				ctx_->addEvent(ev);
				written += meta->position - currentPos;
				if (allWritten(duration, written)) {
					remberPosition(voice, ev, it, originalDuration);
					break;
				}
			}
			return written;
		}

        void StyleRenderer::render(fm::Ticks duration)
        {
            const auto &styleTracks = ctx_->currentStyle().tracks;

			for (const auto &track : styleTracks)
			{
				for (const auto &voice : track->voices)
				{
					if (voice.events.empty() || !hasAnyTimeConsumingEvents(voice.events)) {
						continue;
					}
					setTargetCreateIfNotExists(*track, voice);
					auto meta = ctx_->voiceMetaData();
					fm::Ticks writtenDuration = 0;
					while (!allWritten(duration, writtenDuration)) { // loop until enough events are written
						auto it = voice.events.begin();
						if (hasRemberedPosition(*meta)) { // continue rendering
							it = continueOnRemeberedPosition(voice);
						}
						else if (meta->eventOffset > 0) { // skip events (for upbeat)
							it += meta->eventOffset;
						}
						writtenDuration = renderVoice(voice, it, duration, writtenDuration);
					}
				}
			}
        }
    }
}