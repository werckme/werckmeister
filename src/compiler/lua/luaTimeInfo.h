#ifndef LUA_TIMEINFO_H
#define LUA_TIMEINFO_H

#include <compiler/timeInfo.h>

struct lua_State;

namespace sheet {
    namespace lua {

        class LuaTimeInfo {
        public:
            LuaTimeInfo(const sheet::TimeInfo &timeInfo) : timeInfo_(timeInfo)
            {

            }
            void push(lua_State *L);
        protected:
            const sheet::TimeInfo &timeInfo_;
        };
    }
}

#endif