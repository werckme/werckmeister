#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include "ChordDef.h"
#include <memory>
#include "Event.h"

namespace sheet {
    class VoicingStrategy {
    public:
        ~VoicingStrategy() = default;
		typedef Event::Pitches DegreePitches;
		typedef DegreePitches AbsolutePitches;
		virtual AbsolutePitches get(const DegreePitches &degreePicthes, const ChordDef &chord) = 0;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

#endif