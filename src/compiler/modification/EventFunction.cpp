#include "EventFunction.h"
#include <lua.hpp>
#include <lua/ALuaObject.h>
#include <com/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/lua/luaContext.h>
#include <compiler/context/IContext.h>
#include <lua/luaHelper.h>

namespace compiler
{
    LuaEventFunction::LuaEventFunction(const com::String &path) : Base (path)
    {

    }
    bool LuaEventFunction::canExecute() const
    {
        return LuaBase::hasFunction(LUA_EVENTFUNCTION_FENTRY);
    }

    void LuaEventFunction::assertCanExecute() const
    {
        if (!canExecute())
        {
            FM_THROW(Exception, com::String("missing '") + LUA_EVENTFUNCTION_FENTRY + "' function");
        }
    }

    void LuaEventFunction::execute(IContextPtr ctx, Events &outEvents)
    {
        lua_getglobal(L, LUA_EVENTFUNCTION_FENTRY);
        pushParameters(L, ALuaWithParameter::parameters);
        
        lua::LuaTimeInfo ltimeInfo(ctx->getTimeInfo());
        ltimeInfo.push(L);
        lua::LuaContext luaCtx(*ctx.get());
        luaCtx.push(L);

        call(3, 1);
        auto events = popEvents(ctx);
        outEvents.swap(events);
    }
}