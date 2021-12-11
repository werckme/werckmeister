#ifndef LUA_TIMEINFO_H
#define LUA_TIMEINFO_H

#include <compiler/timeInfo.h>

struct lua_State;

namespace documentModel {
    namespace lua {

        class LuaTimeInfo {
        public:
            LuaTimeInfo(const documentModel::TimeInfo &timeInfo) : timeInfo_(timeInfo)
            {

            }
            void push(lua_State *L);
        protected:
            const documentModel::TimeInfo &timeInfo_;
        };
    }
}

#endif