#include "luaTimeInfo.h"
#include <lua.hpp>

namespace lua
{
    void LuaTimeInfo::push(lua_State *L)
    {
        lua_createtable(L, 0, 0);
        auto top = lua_gettop(L);
        lua_pushstring(L, "quarterPosition");
        lua_pushnumber(L, timeInfo_.quarterPosition);
        lua_settable(L, top);
        lua_pushstring(L, "signatureNumerator");
        lua_pushnumber(L, timeInfo_.signatureNumerator);
        lua_settable(L, top);
        lua_pushstring(L, "sinatureDenominator");
        lua_pushnumber(L, timeInfo_.sinatureDenominator);
        lua_settable(L, top);
        lua_pushstring(L, "bpm");
        lua_pushnumber(L, timeInfo_.bpm);
        lua_settable(L, top);
        lua_pushstring(L, "seconds");
        lua_pushnumber(L, timeInfo_.seconds);
        lua_settable(L, top);
    }
}
