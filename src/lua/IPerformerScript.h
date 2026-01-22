#pragma once

#include <memory>
#include <com/common.hpp>

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
        virtual ~IPerformerScript() = default;
        virtual void onMidiEvent(const com::midi::Event*) = 0;
    };
    typedef std::shared_ptr<IPerformerScript> IPerformerScriptPtr;
}