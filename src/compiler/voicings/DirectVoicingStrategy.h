#pragma once

#include "VoicingStrategy.h"

namespace compiler
{

  /**
     * voicing as notated 
     */
  class DirectVoicingStrategy : public VoicingStrategy
  {
  public:
    typedef VoicingStrategy Base;
    virtual ~DirectVoicingStrategy() = default;
    virtual Pitches get(const documentModel::Event &chord, const documentModel::ChordDef &def, const Degrees &degreeIntervals, const TimeInfo &) override;
    com::IHasParameter::ParametersByNames parameters = {};
    virtual ParametersByNames &getParameters() { return this->parameters; }
  };
}
