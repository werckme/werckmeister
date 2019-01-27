#ifndef SPIELANWEISUNG_LUA_SCRIPT_HPP
#define SPIELANWEISUNG_LUA_SCRIPT_HPP

#include "ASpielanweisung.h"
#include <sheet/lua/ALuaScript.h>
#include <fm/common.hpp>
#include <forward.hpp>

#define LUA_SPIELANWEISUNG_FENTRY "perform"

namespace sheet {
    namespace compiler {
        class LuaSpielanweisung : public ASpielanweisung, public lua::ALuaScript {
        public:
            typedef ASpielanweisung Base;
            typedef lua::ALuaScript LuaBase;
            LuaSpielanweisung(const fm::String &path);
            virtual ~LuaSpielanweisung() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying = false) override;
            virtual bool canExecute() const override;
            lua::LuaMidiContextPtr luaMidiContext();
        private:
            lua::LuaMidiContextPtr _luaCtx;
        };
    }
}

#endif