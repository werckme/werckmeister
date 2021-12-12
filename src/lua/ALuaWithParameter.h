#pragma once

extern "C"
{
    struct lua_State;
    struct luaL_Reg;
}

#include <string>
#include <stdexcept>
#include <com/IHasParameter.h>

#define LUA_ALUA_WITH_PARAMETER_PARAMETERS "parameters"

namespace lua
{
    class ALuaWithParameter
    {
    public:
        virtual com::IHasParameter::ParametersByNames &getParameters(lua_State *);

    protected:
        virtual com::IHasParameter::ParametersByNames popParameters(lua_State *);
        virtual com::Parameter popParameter(lua_State *, int position);
        virtual void pushParameters(lua_State *, const com::IHasParameter::ParametersByNames &);
        com::IHasParameter::ParametersByNames parameters;
    };
}
