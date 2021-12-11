#include "ALuaObject.h"
#include <lua.hpp>


namespace documentModel {

    namespace lua {

        std::string ALuaObject::refId() const
        {
            return std::to_string(reinterpret_cast<long long>(this));
        }
        
        void ALuaObject::push(lua_State *L, const luaL_Reg *functions, size_t numFunctions)
        {
            lua_createtable(L, 0, numFunctions);
            if (numFunctions > 0) {
                luaL_setfuncs(L, functions, 0);
            }
            int top = lua_gettop(L);
            lua_pushstring(L, LUA_OBJECT_REF_TABLE_KEY);
            lua_pushstring(L, refId().c_str());
            lua_settable(L, top);
        }

        ALuaObject::RefId ALuaObject::_getRefIdFromObjectTable(lua_State *L, int idx)
        {
            lua_pushstring(L, LUA_OBJECT_REF_TABLE_KEY);
            lua_gettable(L, idx-1);
            const char *refId = lua_tostring(L, -1);
            lua_pop(L, 1);
            return RefId(refId);
        }
    }
}
