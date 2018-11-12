#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include "ChordDef.h"
#include <memory>

namespace sheet {

    class VoicingStrategy {
    public:
        ~VoicingStrategy() = default;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

#endif