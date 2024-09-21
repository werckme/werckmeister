#include "luaContext.h"
#include <lua.hpp>
#include <compiler/context/IContext.h>
#include <compiler/Instrument.h>
#include <map>

static const char *LUA_CONTEXT_PROPETRY_CURRENT_INSTRUMENT = "getCurrentInstrument";
static const char *LUA_EVENT_PROPETRY_INSTRUMENT = "instrument";
static const char *LUA_INSTRUMENT_PROPERTY_CHILDREN = "children";
static const char *LUA_INSTRUMENT_PROPERTY_NAME = "name";
static const char *LUA_INSTRUMENT_PROPERTY_VOLUME = "volume";
static const char *LUA_INSTRUMENT_PROPERTY_PAN = "pan";
static const char *LUA_INSTRUMENT_PROPERTY_MIDI_CHANNEL = "midiChannel";
static const char *LUA_INSTRUMENT_PROPERTY_MIDI_MSB = "midiMsb";
static const char *LUA_INSTRUMENT_PROPERTY_MIDI_LSB = "midiLsb";
static const char *LUA_INSTRUMENT_PROPERTY_MIDI_PC = "midiPc";

namespace lua
{

    static void pushInstrument(lua_State *L, compiler::IContext &ctx, compiler::AInstrumentDefPtr instrument)
    {
        lua_createtable(L, 4, 0);
        if (!instrument)
        {
            return;
        }
        auto top = lua_gettop(L);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_NAME);
        lua_pushstring(L, instrument->uname.c_str());
        lua_settable(L, top);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_VOLUME);
        lua_pushnumber(L, instrument->volume);
        lua_settable(L, top);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_PAN);
        lua_pushnumber(L, instrument->pan);
        lua_settable(L, top);
        //
        auto instrumentSection = std::dynamic_pointer_cast<compiler::InstrumentSectionDef>(instrument);
        if (instrumentSection)
        {
            lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_CHILDREN);
            lua_createtable(L, instrumentSection->instrumentNames.size(), 0);
            auto childrenTop = lua_gettop(L);
            int index = 1;
            for (const auto &uname : instrumentSection->instrumentNames)
            {
                auto instrumentDef = ctx.getInstrumentDef(uname);
                lua_pushinteger(L, index++);
                pushInstrument(L, ctx, instrumentDef);
                lua_settable(L, childrenTop);
            }
            lua_settable(L, top);
            return;
        }
        //
        auto midiInstrument = std::dynamic_pointer_cast<compiler::MidiInstrumentDef>(instrument);
        if (!midiInstrument)
        {
            return;
        }
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_MIDI_CHANNEL);
        lua_pushinteger(L, midiInstrument->channel);
        lua_settable(L, top);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_MIDI_MSB);
        lua_pushinteger(L, midiInstrument->bankMsb);
        lua_settable(L, top);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_MIDI_LSB);
        lua_pushinteger(L, midiInstrument->bankLsb);
        lua_settable(L, top);
        //
        lua_pushstring(L, LUA_INSTRUMENT_PROPERTY_MIDI_PC);
        lua_pushinteger(L, midiInstrument->pc);
        lua_settable(L, top);
    }

    static int getCurrentInstrument(lua_State *L)
    {
        using namespace lua;
        auto luaContext = ALuaObject::getObject<LuaContext>(L, -1);
        auto top = lua_gettop(L);
        // instrument
        auto& context = luaContext->context;
        auto instrument = context.currentInstrumentDef();
        pushInstrument(L, context, instrument);
        return 1;
    }

    namespace 
    {
        typedef std::map<com::String, com::String> LuaDataMap;
        LuaDataMap _luaDataMap;
    }

    static int setDate(lua_State *L)
    {
        using namespace lua;
        const char *value = lua_tostring(L, -1);
        const char *key = lua_tostring(L, -2);
        lua_pop(L, 2);
        _luaDataMap[key] = value;
        return 0;
    }

    static int getDate(lua_State *L)
    {
        const char *key = lua_tostring(L, -1);
        lua_pop(L, 1);
        auto it = _luaDataMap.find(key);
        if (it == _luaDataMap.end()) 
        {
            lua_pushnil(L);
            return 1;
        }
        lua_pushstring(L, it->second.c_str());
        return 1;
    }

    static const luaL_Reg libfs[] = {
        {"getCurrentInstrument", getCurrentInstrument},
        {"setDate", setDate},
        {"getDate", getDate},
        {NULL, NULL}};

    void LuaContext::push(lua_State *L)
    {
        Base::push(L, libfs, 1);
    }
}
