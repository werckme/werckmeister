#include "LuaMod.h"
#include <lua.hpp>
#include <fm/lua/ALuaObject.h>
#include <sheet/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>

namespace sheet {
    namespace compiler {
    }
}

namespace sheet {
    namespace compiler {
        LuaModification::LuaModification(const fm::String &path) : LuaBase(path)
        {
        }

        bool LuaModification::canExecute() const 
        {
            return LuaBase::hasFunction(LUA_MODIFICATION_FENTRY);
        }

        void LuaModification::assertCanExecute() const 
        {
            if (!canExecute()) {
                FM_THROW(Exception, fm::String("missing '") + LUA_MODIFICATION_FENTRY + "' function");
            }
        }

        void LuaModification::pushArgs(const Event::Args &args)
        {
            lua_createtable(L, 0, 0);
            if (args.size() == 1) { // first arg is the script name
                return;
            }
            auto top = lua_gettop(L);
            auto it = args.begin() + 1;
            for(; it < args.end(); ++it) {
                lua_pushinteger(L, it - args.begin());
                lua_pushstring(L, it->c_str());
                lua_settable(L, top);
            }
        }

        void LuaModification::setArguments(const Event::Args &args)
        {
            this->args_ = args;
        }

        void LuaModification::perform(AContext *ctx, const Event &ev)
        {

        }
    }
}