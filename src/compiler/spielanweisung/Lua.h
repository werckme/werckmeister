#ifndef SPIELANWEISUNG_LUA_SCRIPT_HPP
#define SPIELANWEISUNG_LUA_SCRIPT_HPP

#include "ASpielanweisung.h"
#include <sheet/lua/ALuaScript.h>
#include <fm/common.hpp>

namespace sheet {
    namespace compiler {
        class LuaScript : public ASpielanweisung, public lua::ALuaScript {
        public:
            typedef ASpielanweisung Base;
            typedef lua::ALuaScript LuaBase;
            LuaScript(const fm::String &path);
            virtual ~LuaScript() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying = false) override;
        };
    }
}

#endif