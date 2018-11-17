#ifndef SIMPLE_VOICE_STATEGY_HPP
#define SIMPLE_VOICE_STATEGY_HPP

#include "VoicingStrategy.h"

namespace sheet {

    class DirectVoicingStrategy : public VoicingStrategy {
    public:
		typedef VoicingStrategy Base;
        virtual ~DirectVoicingStrategy() = default;
		virtual Pitches get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals) override;
    };
}

#endif