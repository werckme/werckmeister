#include "Lua.h"
#include <lua.hpp>
#include <sheet/lua/LuaMidiContext.h>

namespace sheet {
    namespace compiler {
        LuaSpielanweisung::LuaSpielanweisung(const fm::String &path) : LuaBase(path)
        {
        }

        bool LuaSpielanweisung::canExecute() const 
        {
            return LuaBase::hasFunction(LUA_SPIELANWEISUNG_FENTRY);
        }

        lua::LuaMidiContextPtr LuaSpielanweisung::luaMidiContext() 
        {
            if (!_luaCtx) {
                _luaCtx = std::make_shared<lua::LuaMidiContext>();
            }
            return _luaCtx;
        }

        void LuaSpielanweisung::addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying)
        {
            lua_getglobal(L, LUA_SPIELANWEISUNG_FENTRY);
            luaMidiContext()->push(L, ctx);
            call(1, 0);
        }
    }
}
