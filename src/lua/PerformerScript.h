#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"
#include <forward.hpp>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <sol/sol.hpp>

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
            com::Byte type = -1;
            com::Byte channel = -1;
            com::Byte metaType = -1;
            com::Byte parameter1 = -1;
            com::Byte parameter2 = -1;
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
        struct NoteOnCacheValue  
        {
            const Output *output;
            int channel;
            int pitch;
            bool operator==(const NoteOnCacheValue&b) const
            {
                return output == b.output 
                    && channel == b.channel
                    && pitch == b.pitch;
            }
        };
        struct NoteOnCacheValueHasher 
        {
            void hash_combine(std::size_t& seed, std::size_t value) const
            {
                seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
            }
            size_t operator()(const NoteOnCacheValue& v) const
            {
                std::size_t h = 0;
                hash_combine(h, std::hash<std::uintptr_t>{}((std::uintptr_t)v.output));
                hash_combine(h, std::hash<int>{}(v.channel));
                hash_combine(h, std::hash<int>{}(v.pitch));
                return h;
            }
        };
        struct LuaMidiInput
        {
            std::string name;
            std::string id;
        };
        typedef std::vector<LuaMidiInput> LuaMidiInputs;
        PerformerScript(com::midi::MidiPtr midiFile) :
              _midiFile(midiFile) 
        {
        }
        virtual ~PerformerScript();
        void scriptPath(const com::String &scriptPath);
        virtual bool canExecute() const override { return false; }
        virtual void assertCanExecute() const override {}
        virtual void onMidiEvent(const Output& output, const com::midi::Event*) override;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction& rq) override { onSeekRequest = rq; }
        virtual void setSendMidiEventHandler(const OnSendMidiEventFunction& sm) override { onSendMidiEvent = sm; }
        virtual void setMidiBackend(app::AMidiBackend* midiBackend) override { _midiBackend = midiBackend; }
        virtual void onTick(com::Ticks ticks) override;
        virtual void init() override;
    private:
        app::AMidiBackend::Inputs _midiInputs;
        void sendNoteOffs();
        void performJump(double position);
        void updateNoteOnCache(const Output& output, const com::midi::Event*);
        double nextJumpToValue = -1;
        app::AMidiBackend* _midiBackend = nullptr;
        std::unordered_set<NoteOnCacheValue, NoteOnCacheValueHasher> noteOnCache;
        com::midi::MidiPtr _midiFile = nullptr;
        OnSeekRequestFunction onSeekRequest = nullptr;
        OnSendMidiEventFunction onSendMidiEvent = nullptr;
        LuaMidiInputs getLuaMidiInputs();
        void listenTo(const std::string &id, sol::protected_function callBack);
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