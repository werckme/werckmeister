#include "LuaMod.h"
#include <lua.hpp>
#include <fm/lua/ALuaObject.h>
#include <sheet/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/AContext.h>

namespace sheet {
    namespace compiler {
        struct LuaEvent : lua::ALuaObject {
            typedef lua::ALuaObject Base;
            const Event *event;
            AContext *ctx;
            LuaEvent(const Event *event, AContext *ctx)
                : event(event), ctx(ctx)
            {}
            void push(lua_State *L);
            void pushPitches(lua_State *L);
            const char *getTypename() const;
        };
        void LuaEvent::push(lua_State *L)
        {
            lua_createtable(L, 2, 0);
            auto top = lua_gettop(L);
            // type
            lua_pushstring(L, "type");
            lua_pushstring(L, getTypename());
            lua_settable(L, top);
            // pitches
            lua_pushstring(L, "notes");
            pushPitches(L);
            lua_settable(L, top);          
        }
        void LuaEvent::pushPitches(lua_State *L)
        {
            lua_createtable(L, event->pitches.size(), 0);
            auto top = lua_gettop(L);
            int count = 1;
            for(const auto &pitch : event->pitches) {
                lua_pushnumber(L, count++);
                lua_createtable(L, event->pitches.size(), 0);
                auto objecttop = lua_gettop(L);
                // pitch
                lua_pushstring(L, "pitch");
                lua_pushnumber(L, pitch.pitch);
                lua_settable(L, objecttop);
                // octave
                lua_pushstring(L, "octave");
                lua_pushnumber(L, pitch.octave);
                lua_settable(L, objecttop);     
                // offset
                lua_pushstring(L, "offset");
                lua_pushnumber(L, 0);
                lua_settable(L, objecttop); 
                // velocity
                lua_pushstring(L, "velocity");
                lua_pushnumber(L, event->velocity);
                lua_settable(L, objecttop);                 
                lua_settable(L, top);                           
            }
        }
        const char * LuaEvent::getTypename() const
        {
            switch (event->type)
            {
            case Event::Note: return "note";
            default: return "unknown";
            }
            return nullptr;
        }
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
            lua_getglobal(L, LUA_MODIFICATION_FENTRY);
            LuaEvent(&ev, ctx).push(L);
            pushArgs(this->args_);
            lua::LuaTimeInfo(ctx->getTimeInfo()).push(L);
            call(3, 0);
        }
    }
}