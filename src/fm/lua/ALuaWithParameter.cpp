#include "ALuaWithParameter.h"
#include <lua.hpp>
#include <fm/exception.hpp>

#define FM_LUA_WITH_PARAMETER_KEY_NAME "name"
#define FM_LUA_WITH_PARAMETER_KEY_POSITION "position"
#define FM_LUA_WITH_PARAMETER_KEY_DEFAULT "default"

namespace fm {
    namespace lua {
        IHasParameter::ParametersByNames & ALuaWithParameter::getParameters(lua_State *L)
        {
            lua_getglobal(L, LUA_ALUA_WITH_PARAMETER_PARAMETERS);
            if (lua_istable(L, -1) != 1) {
                return _parameters;
            }
            try {
                _parameters = popParameters(L);
            } catch (const std::exception &ex) {
                fm::StringStream ss;
                ss << "failed to process lua " << LUA_ALUA_WITH_PARAMETER_PARAMETERS << " table: " << std::endl;
                ss << "    " << ex.what();
                FM_THROW(fm::Exception, ss.str());
            }
            return _parameters;
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

        fm::Parameter ALuaWithParameter::popParameter(lua_State *L, int position)
        {
            // name
            lua_pushstring(L, FM_LUA_WITH_PARAMETER_KEY_NAME);
            lua_gettable(L, -2);
            if (!lua_isstring(L, -1)) {
                FM_THROW(fm::Exception, fm::String("missing parameter string value ") + FM_LUA_WITH_PARAMETER_KEY_NAME);
            }
            
            fm::String name (lua_tostring(L, -1));
            lua_pop(L, 1);

            // default value
            fm::String defaultValue;
            lua_pushstring(L, FM_LUA_WITH_PARAMETER_KEY_DEFAULT);
            lua_gettable(L, -2);
            if (lua_isstring(L, -1)) {
                defaultValue = lua_tostring(L, -1);
                lua_pop(L, 1);
                return fm::Parameter(name, position, defaultValue);
            }
            lua_pop(L, 1);
            fm::Parameter parameter(name, position);
            return parameter;
        }
    }
}