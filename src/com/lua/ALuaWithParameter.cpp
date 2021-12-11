#include "ALuaWithParameter.h"
#include <lua.hpp>
#include <com/exception.hpp>

#define FM_LUA_WITH_PARAMETER_KEY_NAME "name"
#define FM_LUA_WITH_PARAMETER_KEY_POSITION "position"
#define FM_LUA_WITH_PARAMETER_KEY_DEFAULT "default"

namespace com {
    namespace lua {
        IHasParameter::ParametersByNames & ALuaWithParameter::getParameters(lua_State *L)
        {
            lua_getglobal(L, LUA_ALUA_WITH_PARAMETER_PARAMETERS);
            if (lua_istable(L, -1) != 1) {
                return parameters;
            }
            try {
                parameters = popParameters(L);
            } catch (const std::exception &ex) {
                com::StringStream ss;
                ss << "failed to process lua " << LUA_ALUA_WITH_PARAMETER_PARAMETERS << " table: " << std::endl;
                ss << "    " << ex.what();
                FM_THROW(com::Exception, ss.str());
            }
            return parameters;
        }

        IHasParameter::ParametersByNames ALuaWithParameter::popParameters(lua_State *L)
        {
            IHasParameter::ParametersByNames result;
            lua_pushnil(L);
            int pos = 0;
            while (lua_next(L, -2) != 0) {
                if (!lua_istable(L, -1)) {
                    lua_pop(L, 1);
                    continue;
                }
                auto parameter = popParameter(L, pos++);
                result.emplace( std::make_pair(parameter.name(), parameter) );
                lua_pop(L, 1);
            }
            return result;
        }

        com::Parameter ALuaWithParameter::popParameter(lua_State *L, int position)
        {
            // name
            lua_pushstring(L, FM_LUA_WITH_PARAMETER_KEY_NAME);
            lua_gettable(L, -2);
            if (!lua_isstring(L, -1)) {
                FM_THROW(com::Exception, com::String("missing parameter string value ") + FM_LUA_WITH_PARAMETER_KEY_NAME);
            }
            
            com::String name (lua_tostring(L, -1));
            lua_pop(L, 1);

            // default value
            com::String defaultValue;
            lua_pushstring(L, FM_LUA_WITH_PARAMETER_KEY_DEFAULT);
            lua_gettable(L, -2);
            if (lua_isstring(L, -1)) {
                defaultValue = lua_tostring(L, -1);
                lua_pop(L, 1);
                return com::Parameter(name, position, defaultValue);
            }
            lua_pop(L, 1);
            com::Parameter parameter(name, position);
            return parameter;
        }

        void ALuaWithParameter::pushParameters(lua_State *L, const IHasParameter::ParametersByNames &parameters)
        {
            lua_createtable(L, 0, 0);
            if (parameters.empty()) { // first arg is the script name
                return;
            }
            try {
                auto top = lua_gettop(L);
                for(auto parameter : parameters) {
                    lua_pushstring(L, parameter.first.c_str());
                    lua_pushstring(L, parameter.second.value<com::String>().c_str());
                    lua_settable(L, top);
                }
            } catch (const std::exception &ex) {
                com::StringStream ss;
                ss << "failed to push parameter values to lua script:" << std::endl;
                ss << ex.what();
                FM_THROW(com::Exception, ss.str());
            }
        }
    }
}