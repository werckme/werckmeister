#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"
#include <forward.hpp>
#include <vector>

namespace sol
{
    class state_view;
}

namespace lua
{
    class PerformerScript : public IPerformerScript, public ALuaScript
    {
    public:
        typedef std::string LuaMidiData;
        struct LuaMidi
        {
            float position = 0;
            int type = -1;
            int channel = -1;
            int metaType = -1;
            LuaMidiData data;
        };
        typedef std::vector<LuaMidi> LuaMidiEvents;
        struct LuaMidiTrack
        {
            std::string name;
            LuaMidiEvents events;
        };
        typedef std::vector<LuaMidiTrack> LuaMidiTracks;
        typedef ALuaScript Base;
        PerformerScript(com::midi::MidiPtr midiFile) : 
            _midiFile(midiFile) 
        {
        }
        virtual ~PerformerScript();
        void scriptPath(const com::String &scriptPath);
        virtual bool canExecute() const override { return false; }
        virtual void assertCanExecute() const override {}
        virtual void onMidiEvent(const com::midi::Event*) override;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction& rq) override { onSeekRequest = rq; }
        virtual void init() override;
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
        LuaMidiTracks hostGetMidiEvents() const;
    };
    typedef std::shared_ptr<PerformerScript> PerformerScriptPtr;
}