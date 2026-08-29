#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"
#include <forward.hpp>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <sol/sol.hpp>
#include <optional>
#include <mutex>
#include <queue>


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
            bool isModified = false;
            float position = 0;
            com::Byte type = -1;
            com::Byte channel = -1;
            com::Byte metaType = -1;
            com::Byte parameter1 = -1;
            com::Byte parameter2 = -1;
            int id = 0;
            LuaMidiData data;
            inline void modified() { isModified = true; }
        };
        typedef std::vector<LuaMidi> LuaMidiEvents;
        struct LuaMidiTrack
        {
            std::string name;
            LuaMidiEvents events;
        };
        typedef std::vector<LuaMidiTrack> LuaMidiTracks;
        typedef ALuaScript Base;
        struct MidiEventWithOutput
        {
            const Output *output;
            com::midi::Event event;
        };
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
        void script(const com::String &scriptText);
        virtual void enqueue(const Output* output, com::midi::Event) override;
        virtual bool canExecute() const override { return false; }
        virtual void assertCanExecute() const override {}
        virtual void onMidiEvent(const Output& output, const com::midi::Event*, com::midi::Event **outEventPtrContainer) override;
        virtual void setSeekRequestHandler(const OnSeekRequestFunction& rq) override { onSeekRequest = rq; }
        virtual void setSendMidiEventHandler(const OnSendMidiEventFunction& sm) override { onSendMidiEvent = sm; }
        virtual void setMidiBackend(app::AMidiBackend* midiBackend) override { _midiBackend = midiBackend; }
        virtual void onTick(com::Ticks ticks) override;
        virtual void init() override;
    protected:
        com::String _script;
        app::AMidiBackend::Inputs _midiInputs;
        typedef std::queue<MidiEventWithOutput> EventQueue;
        EventQueue _eventQueue;
        typedef std::recursive_mutex QueueLock;
        QueueLock _queueLock;
        void processEventQueue();
        virtual void sendNoteOffs();
        virtual void sendNoteOffs(int channel);
        virtual void performJump(double position);
        virtual void updateNoteOnCache(const Output& output, const com::midi::Event*);
        double nextJumpToValue = -1;
        app::AMidiBackend* _midiBackend = nullptr;
        std::unordered_set<NoteOnCacheValue, NoteOnCacheValueHasher> noteOnCache;
        com::midi::MidiPtr _midiFile = nullptr;
        OnSeekRequestFunction onSeekRequest = nullptr;
        OnSendMidiEventFunction onSendMidiEvent = nullptr;
        LuaMidi createLuaMidiFrom(const com::midi::Event &midiEvent) const;
        com::midi::Event createMidiFrom(const LuaMidi& luaMidi) const;
        virtual LuaMidiInputs getLuaMidiInputs();
        virtual void listenTo(const std::string &id, sol::protected_function callBack);
        void jumpToPosition(double quarters);
        virtual void initLuaFunctions(sol::state_view&);
        virtual void initLuaTypes(sol::state_view&);
        sol::state_view* luaPtr = nullptr;
        /////////////////////
        typedef std::optional<LuaMidi> LuaMidiOptional;
        std::function<LuaMidiOptional(LuaMidi)> luaOnMidiEvent = [](auto x){return x;};
        std::function<void(double)> luaOnTick = nullptr;
        std::function<void()> luaInit = nullptr;
        LuaMidiTracks hostGetMidiEvents() const;
        typedef std::unordered_map<int, int> TagMap;
        TagMap tagMap;
    };
    typedef std::shared_ptr<PerformerScript> PerformerScriptPtr;
}