#include "PerformerScript.h"
#include <sol/sol.hpp>
#include <com/midi.hpp>

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
        sol::function luaF = lua["onMidiEvent"];
        if (luaF.is<sol::nil_t>() == false)
        {
            luaOnMidiEvent = luaF;
        }
    }

    void PerformerScript::initLuaTypes(sol::state_view& lua)
    {
        lua.new_usertype<LuaMidi>("LuaMidi",
            "position", &LuaMidi::position
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