#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include "sheet/ChordDef.h"
#include "sheet/Event.h"
#include <memory>


namespace sheet {

    class VoicingStrategy {
    public:
        struct TimeInfo {};
		typedef Event::Pitches Degrees;
		typedef Degrees Pitches;
		virtual Pitches get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) = 0;
        virtual ~VoicingStrategy() = default;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

#endif