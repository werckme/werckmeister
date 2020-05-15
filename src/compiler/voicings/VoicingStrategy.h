#ifndef VOICE_STATEGY_HPP
#define VOICE_STATEGY_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/objects/Event.h>
#include <memory>
#include <fm/common.hpp>
#include <compiler/timeInfo.h>

namespace sheet {

    class VoicingStrategy {
    public:
		typedef Event::Pitches Degrees;
		typedef Degrees Pitches;
		virtual Pitches get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) = 0;
        virtual ~VoicingStrategy() = default;
        virtual void setArguments(const Event::Args &args) {}
        fm::String name() const { return name_; }
        void name(const fm::String &name) { name_ = name; }
    private:
        fm::String name_;
    };
    typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

#endif