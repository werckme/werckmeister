#ifndef SHEET_ALUA_SCRIPT_H
#define SHEET_ALUA_SCRIPT_H

#include <memory>
#include <fm/common.hpp>

extern "C" {
    struct lua_State;
}

namespace sheet {

    namespace lua {

        class ALuaScript {
        public:
            ALuaScript(const fm::String &path);
            virtual ~ALuaScript();
            bool hasFunction(const fm::String &name);
        private:
            lua_State *L = nullptr;
        };
    }
}

#endif
