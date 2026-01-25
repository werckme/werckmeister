#pragma once

#include <memory>
#include <com/common.hpp>
#include <functional>

namespace com 
{
    namespace midi
    {
        struct Event;
    }
}

namespace lua
{
    class IPerformerScript 
    {
    public:
        typedef std::function<void(double)> OnSeekRequestFunction;
        virtual ~IPerformerScript() = default;
        virtual void onMidiEvent(const com::midi::Event*) = 0;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction&) = 0;
        virtual void init() = 0;
    };
    typedef std::shared_ptr<IPerformerScript> IPerformerScriptPtr;
}