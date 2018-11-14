#ifndef SIMPLE_VOICE_STATEGY_HPP
#define SIMPLE_VOICE_STATEGY_HPP

#include "VoicingStrategy.h"

namespace sheet {

    class SimpleVoicingStrategy : public VoicingStrategy {
    public:
		typedef VoicingStrategy Base;
		virtual AbsolutePitches get(const DegreePitches &degreePicthes, const ChordDef &chord) override;
        ~SimpleVoicingStrategy() = default;
    };
}

#endif