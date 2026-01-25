#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"
#include <forward.hpp>

namespace sol
{
    class state_view;
}

namespace lua
{
    class PerformerScript : public IPerformerScript, public ALuaScript
    {
    public:
        struct LuaMidi
        {
            float position = 0;
        };
        typedef ALuaScript Base;
        PerformerScript(com::midi::MidiPtr midiFile) : 
            _midiFile(midiFile) 
        {
        }
        virtual ~PerformerScript();
        void scriptPath(const com::String &scriptPath);
        virtual bool canExecute() const { return false; }
        virtual void assertCanExecute() const {}
        virtual void onMidiEvent(const com::midi::Event*) override;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction& rq) override { onSeekRequest = rq; }
        virtual void init();
    private:
        com::midi::MidiPtr _midiFile = nullptr;
        OnSeekRequestFunction onSeekRequest = nullptr;
        void jumpToPosition(double quarters);
        void initLuaFunctions(sol::state_view&);
        void initLuaTypes(sol::state_view&);
        sol::state_view* luaPtr = nullptr;
        /////////////////////
        std::function<void(LuaMidi)> luaOnMidiEvent = nullptr;
        std::function<void()> luaInit = nullptr;
        void hostGetNumMidiEvents();
    };
    typedef std::shared_ptr<PerformerScript> PerformerScriptPtr;
}