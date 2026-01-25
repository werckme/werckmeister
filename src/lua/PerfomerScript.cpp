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
        sol::function luaF = lua["OnMidiEvent"];
        if (luaF.valid())
        {
            luaOnMidiEvent = [luaF](LuaMidi midiEv) {
                auto result = luaF(midiEv);
                if (!result.valid())
                {
                    sol::error err = result;
                    throw std::runtime_error(err.what());
                }
            };
        }

        lua["JumpToPosition"] = [this](double position)
        {
            onSeekRequest(position);
        };
    }

    void PerformerScript::initLuaTypes(sol::state_view& lua)
    {
        lua.new_usertype<LuaMidi>("LuaMidi",
            "position", sol::property(&LuaMidi::position)
        );
    }

    void PerformerScript::onMidiEvent(const com::midi::Event* ev)
    {
        LuaMidi luaMidi;
        luaMidi.position = ev->absPosition() / com::PPQ;
        luaOnMidiEvent(luaMidi);
    }

    PerformerScript::~PerformerScript()
    {
        if (luaPtr)
        {
            delete luaPtr;
        }
    }
}