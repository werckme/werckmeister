#include "Track.h"
#include <algorithm>

namespace documentModel {
    com::Ticks Track::maxLength() const
    {
		com::Ticks max = 0;
        for (const auto &voice : voices) {
			com::Ticks voiceLength = 0;
			for(const auto &ev : voice.events) {
				if (!ev.isTimeConsuming()) {
					continue;
				}
				voiceLength += ev.duration;
			}
			max = std::max(max, voiceLength);
		}
		return max;
    }
}