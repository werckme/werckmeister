#pragma once

#include <lua.hpp>
#include <com/common.hpp>

namespace lua
{

    inline const char * getVersion() 
    {
        static const char * versionStr = LUA_VERSION;
        return versionStr;
    }
    inline void getTableValue(lua_State *L, const char *key, int &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }
    inline void getTableValue(lua_State *L, const char *key, bool &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_toboolean(L, -1);
        lua_pop(L, 1);
    }
    inline void getTableValue(lua_State *L, const char *key, double &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    inline void getTableValue(lua_State *L, const char *key, long double &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    inline void getTableValue(lua_State *L, const char *key, float &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    inline void getTableValue(lua_State *L, const char *key, com::String &out, bool required = true)
    {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!required && lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        out = lua_tostring(L, -1);
        lua_pop(L, 1);
    }

    inline void setTableValue(lua_State *L, const char *key, int stackIndex, int value)
    {
        lua_pushstring(L, key);
        lua_pushnumber(L, value);
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, bool value)
    {
        lua_pushstring(L, key);
        lua_pushboolean(L, value);
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, double value)
    {
        lua_pushstring(L, key);
        lua_pushnumber(L, value);
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, long double value)
    {
        lua_pushstring(L, key);
        lua_pushnumber(L, value);
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, float value)
    {
        lua_pushstring(L, key);
        lua_pushnumber(L, value);
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, const com::String &value)
    {
        lua_pushstring(L, key);
        lua_pushstring(L, value.c_str());
        lua_settable(L, stackIndex);
    }
    inline void setTableValue(lua_State *L, const char *key, int stackIndex, const char *value)
    {
        lua_pushstring(L, key);
        lua_pushstring(L, value);
        lua_settable(L, stackIndex);
    }
}
