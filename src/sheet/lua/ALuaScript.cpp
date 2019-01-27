#include "ALuaScript.h"
#include <lua.hpp>

namespace sheet {
    
    namespace lua {

        ALuaScript::ALuaScript(const fm::String &path)
        {
            L= luaL_newstate();
            luaL_openlibs(L); 
        }

        ALuaScript::~ALuaScript()
        {
            lua_close(L);
        }

        bool ALuaScript::hasFunction(const fm::String &name)
        {
            return false;
        }
    }
}