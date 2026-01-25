#include "PerformerScript.h"
#include <sol/sol.hpp>
#include <com/midi.hpp>
#include <iostream>

namespace
{
    lua::PerformerScript::LuaMidi createFrom(const com::midi::Event &midiEvent)
    {
        lua::PerformerScript::LuaMidi result;
        result.position = midiEvent.absPosition() / com::PPQ;
        result.type = midiEvent.eventType();
        if (midiEvent.eventType() == com::midi::MetaEvent)
        {
            result.metaType = midiEvent.metaEventType();
            result.data = lua::PerformerScript::LuaMidiData(midiEvent.metaData(), midiEvent.metaData() + midiEvent.metaDataSize());
        } 
        else 
        {
            result.channel = midiEvent.channel();
            result.data = lua::PerformerScript::LuaMidiData({
                (char)midiEvent.parameter1(),
                (char)midiEvent.parameter2(),
            });
        }
        return result;
    }
}

namespace lua
{
    void PerformerScript::scriptPath(const com::String &scriptPath)
    {
        _path = scriptPath;
    }

    void PerformerScript::initLuaFunctions(sol::state_view& lua)
    {
        // LUA
        sol::function solOnMidiEvent = lua["OnMidiEvent"];
        if (solOnMidiEvent.valid())
        {
            luaOnMidiEvent = [solOnMidiEvent](LuaMidi midiEv) {
                auto result = solOnMidiEvent(midiEv);
                if (!result.valid())
                {
                    sol::error err = result;
                    throw std::runtime_error(err.what());
                }
            };
        }
        sol::function solInit = lua["Init"];
        if (solInit.valid())
        {
            luaInit = [solInit]() {
                auto result = solInit();
                if (!result.valid())
                {
                    sol::error err = result;
                    throw std::runtime_error(err.what());
                }
            };
        }

        /// HOST
        lua["JumpToPosition"] = [this](double position)
        {
            onSeekRequest(position);
        };
        lua["GetMidiEvents"] = [this]()
        {
            return hostGetMidiEvents();
        };
    }

    void PerformerScript::initLuaTypes(sol::state_view& lua)
    {
        lua.new_usertype<LuaMidi>("LuaMidi",
            "position", sol::property(
                [](const LuaMidi& m) {
                    return m.position;
                }
            ),
            "type", sol::property(
                [](const LuaMidi& m) {
                    return m.type;
                }
            ),
            "channel", sol::property(
                [](const LuaMidi& m) {
                    return m.channel;
                }
            ),
            "metaType", sol::property(
                [](const LuaMidi& m) {
                    return m.metaType;
                }
            ),
            "data", sol::property(
                [](const LuaMidi& m) {
                    return m.data;
                }
            )
        );
        lua.new_usertype<LuaMidiTrack>("LuaMidiTrack",
            "name", sol::property(
                [](const LuaMidiTrack& m) {
                    return m.name;
                }
            ),
            "events", sol::property(
                [](const LuaMidiTrack& m) {
                    return m.events;
                }
            )
        );
    }

    void PerformerScript::onMidiEvent(const com::midi::Event* ev)
    {
        LuaMidi luaMidi = createFrom(*ev);
        luaOnMidiEvent(luaMidi);
    }

    void PerformerScript::init()
    {
        prepareLuaEnvironment();
        luaPtr = new sol::state_view(L);
        luaPtr->script_file(_path);
        initLuaTypes(*luaPtr);
        initLuaFunctions(*luaPtr);
        if (luaInit)
        {
            luaInit();
        }
    }

    PerformerScript::~PerformerScript()
    {
        if (luaPtr)
        {
            delete luaPtr;
        }
    }

    PerformerScript::LuaMidiTracks PerformerScript::hostGetMidiEvents() const
    {
        LuaMidiTracks luaTracks;
        luaTracks.reserve( _midiFile->ctracks().size());
        for(const auto& track : _midiFile->ctracks())
        {
            LuaMidiTrack luaTrack;
            luaTrack.events.reserve(track->events().numEvents());
            luaTrack.name = "";
            for(const auto& event : track->events())
            {
                if (event.eventType() == com::midi::MetaEvent && event.metaEventType() == com::midi::SequenceOrTrackName)
                {
                    std::string name((char*)event.metaData(), event.metaDataSize());
                    luaTrack.name = name;
                }
                LuaMidi luaEvent = createFrom(event);
                luaTrack.events.emplace_back(luaEvent);
            }
            luaTracks.emplace_back(luaTrack);
        }
        return luaTracks;
    }
}