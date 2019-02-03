#include "styleRenderer.h"

namespace sheet {
    namespace compiler {
        void StyleRenderer::render(AContextPtr ctx, fm::Ticks duration)
        {
            auto styleTracks = ctx->currentStyle();

			for (const auto &track : *styleTracks)
			{
				for (const auto &voice : track.voices)
				{
					if (voice.events.empty()) {
						continue;
					}
					ctx->setTarget(track, voice);
					auto meta = ctx->voiceMetaData();
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
							ctx->addEvent(ev);
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