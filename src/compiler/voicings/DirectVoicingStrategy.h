#ifndef SIMPLE_VOICE_STATEGY_HPP
#define SIMPLE_VOICE_STATEGY_HPP

#include "VoicingStrategy.h"

namespace documentModel {

    /**
     * voicing as notated 
     */
    class DirectVoicingStrategy : public VoicingStrategy {
    public:
		  typedef VoicingStrategy Base;
      virtual ~DirectVoicingStrategy() = default;
      virtual Pitches get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) override;
      com::IHasParameter::ParametersByNames parameters = {};
      virtual ParametersByNames & getParameters() { return this->parameters; }
    };
}

#endif