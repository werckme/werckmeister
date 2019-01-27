#ifndef SHEET_LUA_CONTEXT_H
#define SHEET_LUA_CONTEXT_H

#include <forward.hpp>
#include "ALuaObject.h"
#include <fm/units.hpp>

extern "C" {
    struct lua_State;
}

namespace sheet {

    namespace lua {

        class LuaMidiContext : public ALuaObject {
        public:
            typedef ALuaObject Base;
            LuaMidiContext() = default;
            LuaMidiContext(const LuaMidiContext&) = delete;
            LuaMidiContext & operator=(const LuaMidiContext&) = delete;
            virtual ~LuaMidiContext() = default;
            void push(lua_State *L, sheet::compiler::AContext *ctx);
            // callbacks
            fm::Ticks barLength() const;
        private:
            sheet::compiler::AContext *_ctx = nullptr;
        };
    }
}

#endif
