#include "preprocessor.h"
#include <sheet/Track.h>
#include <compiler/metaData.h>

namespace sheet {
	namespace compiler {

        void Preprocessor::process(Track &track)
        {
            for (auto &voice : track.voices)
			{
				auto it = voice.events.begin();
				auto end = voice.events.end();
				fm::Ticks lastDuration = VoiceMetaData::DefaultDuration;
				for (; it!=end; ++it)
				{
					auto &ev = *it;
					if (ev.isTimeConsuming()) {
						if (ev.duration == 0) {
							ev.duration = lastDuration;
						} else {
							lastDuration = ev.duration;
						}
					}
				}
			}
        }

    }
}