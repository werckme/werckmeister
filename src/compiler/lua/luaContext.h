#pragma once

#include <lua/ALuaObject.h>

struct lua_State;

namespace compiler
{
	class IContext;
}

namespace lua
{

    class LuaContext : public lua::ALuaObject
    {
    public:
        typedef lua::ALuaObject Base;
        LuaContext(compiler::IContext& context) : context(context)
        {
        }
        void push(lua_State *L);
        compiler::IContext &context;
        virtual ~LuaContext() = default;
    };
}
