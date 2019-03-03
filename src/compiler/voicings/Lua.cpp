#include "Lua.h"
#include <lua.hpp>

namespace sheet {
    namespace compiler {
        LuaVoicingStrategy::LuaVoicingStrategy(const fm::String &path) : LuaBase(path)
        {
        }

        bool LuaVoicingStrategy::canExecute() const 
        {
            return LuaBase::hasFunction(LUA_VOICING_STRATEGY_FENTRY);
        }

        LuaVoicingStrategy::Pitches LuaVoicingStrategy::get(const Event &chord, 
            const ChordDef &def, 
            const Degrees &degreeIntervals, 
            const TimeInfo&)
        {
            lua_getglobal(L, LUA_VOICING_STRATEGY_FENTRY);
            call(1, 0);
            return Pitches();
        }

        // void LuaSpielanweisung::addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying)
        // {
        //     lua_getglobal(L, LUA_SPIELANWEISUNG_FENTRY);
        //     luaMidiContext()->push(L, ctx);
        //     call(1, 0);
        // }
    }
}
