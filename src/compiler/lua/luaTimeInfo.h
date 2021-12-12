#pragma once

#include <compiler/timeInfo.h>

struct lua_State;

namespace lua
{

    class LuaTimeInfo
    {
    public:
        LuaTimeInfo(const documentModel::TimeInfo &timeInfo) : timeInfo_(timeInfo)
        {
        }
        void push(lua_State *L);

    protected:
        const documentModel::TimeInfo &timeInfo_;
    };
}
