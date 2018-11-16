#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include "ChordDef.h"
#include "Event.h"
#include <memory>


namespace sheet {

    class VoicingStrategy {
    public:
		typedef ChordDef::Intervals DegreeIntervals;
		typedef ChordDef::Intervals PitchIntervals;
		virtual PitchIntervals get(const ChordDef &def, const DegreeIntervals &degreeIntervals) = 0;
        virtual ~VoicingStrategy() = default;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

#endif