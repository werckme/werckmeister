#include "PerformerScript.h"
#include <com/midi.hpp>
#include <iostream>
#include <algorithm>

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
            result.parameter1 = midiEvent.parameter1();
            result.parameter2 = midiEvent.parameter2();
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
            nextJumpToValue = position;
        };
        lua["GetMidiEvents"] = [this]()
        {
            return hostGetMidiEvents();
        };
        lua["GetMidiInputs"] = [this]()
        {
            return getLuaMidiInputs();
        };
        lua["AddMidiInputListener"] = [this](const std::string &id, sol::function callBack)
        {
            return listenTo(id, callBack);
        };
    }

    void PerformerScript::performJump(double position)
    {
        onSeekRequest(position);
        sendNoteOffs();
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
            ),
            "parameter1", sol::property(
                [](const LuaMidi& m) {
                    return m.parameter1;
                }
            ),
            "parameter2", sol::property(
                [](const LuaMidi& m) {
                    return m.parameter2;
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
        lua.new_usertype<LuaMidiInput>("LuaMidiInput",
            "name", sol::property(
                [](const LuaMidiInput& m) {
                    return m.name;
                }
            ),
            "id", sol::property(
                [](const LuaMidiInput& m) {
                    return m.id;
                }
            )
        );        
        
    }

    void PerformerScript::updateNoteOnCache(const Output& output, const com::midi::Event* ev)
    {
        bool noteOn = ev->eventType() == com::midi::NoteOn;
        bool noteOff = ev->eventType() == com::midi::NoteOff;
        if (!noteOn && !noteOff)
        {
            return;
        }
        NoteOnCacheValue cacheValue;
        cacheValue.output = &output;
        cacheValue.channel = ev->channel();
        cacheValue.pitch = ev->parameter1();
        if (noteOn)
        {
            noteOnCache.insert(cacheValue);
            return;
        }
        noteOnCache.erase(cacheValue);
    }

    void PerformerScript::sendNoteOffs()
    {
        for(const auto noteCacheValue : noteOnCache)
        {
            com::midi::Event midiEv;
            midiEv.eventType(com::midi::NoteOff);
            midiEv.parameter1(noteCacheValue.pitch);
            midiEv.parameter2(0);
            midiEv.channel(noteCacheValue.channel);
            onSendMidiEvent(noteCacheValue.output, &midiEv);
        }
        noteOnCache.clear();
    }

    void PerformerScript::onTick(com::Ticks ticks)
    {
    }

    void PerformerScript::onMidiEvent(const Output& output, const com::midi::Event* ev)
    {
        LuaMidi luaMidi = createFrom(*ev);
        luaOnMidiEvent(luaMidi);
        if (nextJumpToValue >= 0)
        {
            performJump(nextJumpToValue);
        }
        nextJumpToValue = -1;
        updateNoteOnCache(output, ev);
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

    PerformerScript::LuaMidiInputs PerformerScript::getLuaMidiInputs()
    {
        LuaMidiInputs result;
        if (_midiBackend == nullptr)
        {
            return result;
        }
        _midiInputs = _midiInputs.empty() ? _midiBackend->getInputs() : _midiInputs;
        result.reserve(_midiInputs.size());
        for(const auto& input : _midiInputs)
        {
            LuaMidiInput luaIn;
            luaIn.id = input.id;
            luaIn.name = input.name;
            result.emplace_back(luaIn);
        }
        return result;
    }

    void PerformerScript::listenTo(const std::string &id, sol::protected_function callBack)
    {
         _midiInputs = _midiInputs.empty() ? _midiBackend->getInputs() : _midiInputs;
        auto inputIt = std::find_if(_midiInputs.begin(), _midiInputs.end(), [id](const auto &el) { return el.id == id; });
        if (inputIt == _midiInputs.end())
        {
            throw std::runtime_error("invalid midi input id: " + id);
        }
        app::AMidiBackend::Input* inputPtr = &(*inputIt);
        inputPtr->midiMessageCallback = [callBack](const auto *msg)
        {
            std::vector<com::Byte> midiData = {0x0};
            midiData.insert( midiData.end(), msg->begin(), msg->end() );
            com::midi::Event midiEvent;
            midiEvent.read(0, midiData.data(), midiData.size());
            auto luaMidiEvent = createFrom(midiEvent);
            auto result = callBack(const_cast<LuaMidi*>(&luaMidiEvent));
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Error: " << err.what() << std::endl;
            }
        };
        _midiBackend->listenTo(inputPtr);
    }
}