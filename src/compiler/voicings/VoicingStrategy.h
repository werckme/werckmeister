#pragma once

#include <documentModel/objects/ChordDef.h>
#include <documentModel/objects/Event.h>
#include <memory>
#include <com/common.hpp>
#include <com/Parameter.h>
#include <compiler/timeInfo.h>
#include <com/IRegisterable.h>
#include <com/AConvertsArgumentsToParameter.h>

namespace compiler
{

    class VoicingStrategy : public com::IRegisterable, public com::AConvertsArgumentsToParameter
    {
    public:
        typedef documentModel::Event::Pitches Pitches;
        virtual Pitches solve(const documentModel::Event &chord, const documentModel::ChordDef &def, const Pitches &degreePitches, const TimeInfo &) = 0;
        virtual ~VoicingStrategy() = default;
        com::String name() const { return name_; }
        void name(const com::String &name) { name_ = name; }

    private:
        com::String name_;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}
