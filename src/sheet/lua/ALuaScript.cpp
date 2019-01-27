#include "ALuaScript.h"
#include <lua.hpp>
#include <exception>
#include <sstream>

namespace sheet {
    
    namespace lua {

        ALuaScript::ALuaScript(const fm::String &path) : _path(path)
        {
            L= luaL_newstate();
            luaL_openlibs(L);
            if (luaL_dofile(L, fm::to_string(path).c_str())) {
                error(std::string(lua_tostring(L, -1)));
            }
        }

        ALuaScript::~ALuaScript()
        {
            lua_close(L);
        }

        bool ALuaScript::hasFunction(const std::string &name) const 
        {
            lua_getglobal(L, name.c_str());
            if (lua_isfunction(L, -1) == 1) {
                return true;
            }
            return false;
        }

        void ALuaScript::error(const std::string &msg)
        {
            std::stringstream ss;
            ss << fm::to_string(_path) << ": ";
            ss << msg;
            throw std::runtime_error(ss.str());
        }

        void ALuaScript::call(size_t numArgs, size_t numResult)
        {
            if (lua_pcall(L, numArgs, numArgs, 0)) {
                error(std::string(lua_tostring(L, -1)));
            }
        }
    }
}