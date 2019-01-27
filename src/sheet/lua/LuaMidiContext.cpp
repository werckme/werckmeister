#include "LuaMidiContext.h"
#include <compiler/AContext.h>
#include <lua.hpp>
#include <fm/config.hpp>


static int version (lua_State *L) {
    lua_pushstring(L, SHEET_VERSION);
    return 1;
}

static int barLength (lua_State *L) {
    using namespace sheet::lua;
    auto luaMidiContext = ALuaObject::getObject<LuaMidiContext>(L, -1);
    lua_pushinteger(L, luaMidiContext->barLength());
    return 1;
}


static const luaL_Reg libfs[] = {
   {"version",   version},
   {"barLength", barLength},
   {NULL, NULL}
};

namespace sheet {

    namespace lua {

        void LuaMidiContext::push(lua_State *L, sheet::compiler::AContext *ctx)
        {
            Base::push(L, &libfs[0], 1);
            _ctx = ctx;
        }

        fm::Ticks LuaMidiContext::barLength() const
        {
            auto meta = _ctx->voiceMetaData(_ctx->voice());
            return meta->barLength;
        }
   
    }
}