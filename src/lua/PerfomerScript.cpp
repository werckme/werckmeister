#include "PerformerScript.h"
#include <sol/sol.hpp>
#include <com/midi.hpp>
#include <iostream>
namespace lua
{
    void PerformerScript::scriptPath(const com::String &scriptPath)
    {

        prepareLuaEnvironment();
        luaPtr = new sol::state_view(L);
        luaPtr->script_file(scriptPath);
        initLuaTypes(*luaPtr);
        initLuaFunctions(*luaPtr);
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
    }

    void PerformerScript::initLuaTypes(sol::state_view& lua)
    {
        lua.new_usertype<LuaMidi>("LuaMidi", "position", &LuaMidi::position);
    }

    void PerformerScript::onMidiEvent(const com::midi::Event* ev)
    {
        LuaMidi luaMidi;
        luaMidi.position = ev->absPosition() / com::PPQ;
        luaOnMidiEvent(luaMidi);
    }

    void PerformerScript::init()
    {
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
}