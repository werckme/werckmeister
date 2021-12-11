#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include <documentModel/objects/ChordDef.h>
#include <documentModel/objects/Event.h>
#include <memory>
#include <com/common.hpp>
#include <com/Parameter.h>
#include <compiler/timeInfo.h>
#include <com/IRegisterable.h>
#include <com/AConvertsArgumentsToParameter.h>

namespace documentModel {

    class VoicingStrategy : public com::IRegisterable, public com::AConvertsArgumentsToParameter {
    public:
		typedef Event::Pitches Degrees;
		typedef Degrees Pitches;
		virtual Pitches get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) = 0;
        virtual ~VoicingStrategy() = default;
        com::String name() const { return name_; }
        void name(const com::String &name) { name_ = name; }
    private:
        com::String name_;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}


#endif