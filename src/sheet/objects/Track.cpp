#include "Track.h"
#include <algorithm>

namespace sheet {
    fm::Ticks Track::maxLength() const
    {
		fm::Ticks max = 0;
        for (const auto &voice : voices) {
			fm::Ticks voiceLength = 0;
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