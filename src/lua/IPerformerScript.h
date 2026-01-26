#pragma once

#include <memory>
#include <com/common.hpp>
#include <functional>
#include <app/AMidiBackend.h>

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
        typedef app::AMidiBackend::Output Output;
        typedef std::function<void(double)> OnSeekRequestFunction;
        typedef std::function<void(const Output* output, const com::midi::Event*)> OnSendMidiEventFunction;
        virtual ~IPerformerScript() = default;
        virtual void onMidiEvent(const Output& output, const com::midi::Event*) = 0;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction&) = 0;
        virtual void setSendMidiEventHandler(const OnSendMidiEventFunction&) = 0;
        virtual void setMidiBackend(app::AMidiBackend* midiBackend) = 0;
        virtual void init() = 0;
    };
    typedef std::shared_ptr<IPerformerScript> IPerformerScriptPtr;
}