#include "LuaMod.h"
#include <lua.hpp>
#include <fm/lua/ALuaObject.h>
#include <sheet/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/AContext.h>
#include <fm/lua/luaHelper.h>

static const char * LUA_EVENT_TYPE_NOTE = "note";
static const char * LUA_EVENT_TYPE_UNKNOWN = "unknown";
static const char * LUA_EVENT_PROPETRY_VELOCITY = "velocity";
static const char * LUA_EVENT_PROPETRY_DURATION = "duration";
static const char * LUA_EVENT_PROPETRY_TYING = "isTied";
static const char * LUA_EVENT_PROPETRY_OFFSET = "offset";
static const char * LUA_EVENT_PROPETRY_PITCHES = "pitches";
static const char * LUA_EVENT_PROPETRY_TYPE = "type";
static const char * LUA_EVENT_PITCH_PROPETRY_PITCH = "pitch";
static const char * LUA_EVENT_PITCH_PROPETRY_OCTAVE = "octave";

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
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_TYPE, top, getTypename());
            // pitches
            lua_pushstring(L, LUA_EVENT_PROPETRY_PITCHES);
            pushPitches(L);
            lua_settable(L, top);
            // offset
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_OFFSET, top, 0);            
            // velocity
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, top, event->velocity);      
            // duration
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_DURATION, top, event->duration / fm::PPQ);
            // is tied
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_TYING, top, event->isTying());
                             
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
                lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_PITCH);
                lua_pushnumber(L, pitch.pitch);
                lua_settable(L, objecttop);
                // octave
                lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_OCTAVE);
                lua_pushnumber(L, pitch.octave);
                lua_settable(L, objecttop);  
                lua_settable(L, top);                                
            }
        }
        const char * LuaEvent::getTypename() const
        {
            switch (event->type)
            {
            case Event::TiedNote:
            case Event::Note: return LUA_EVENT_TYPE_NOTE;
            default: return LUA_EVENT_TYPE_UNKNOWN;
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

        void LuaModification::performNoteResult(AContext *ctx)
        {
            double velocity = 0;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, velocity);
            fm::Ticks offset = 0;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_OFFSET, offset);
            offset *= fm::PPQ;
            fm::Ticks duration = 0;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_DURATION, duration);
            bool isTied = false;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_TYING, isTied);
            duration *= fm::PPQ;            
            lua_pushstring(L, LUA_EVENT_PROPETRY_PITCHES);
            lua_gettable(L, -2);
            if (!lua_istable(L, -1)) {
                FM_THROW(Exception, "missing pitches");
            }
            lua_pushnil(L);
            ctx->seek(offset);
            while (lua_next(L, -2) != 0) {
                int pitch = 0;
                sheet::lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_PITCH, pitch);
                int octave = 0;
                sheet::lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_OCTAVE, octave);
                lua_pop(L, 1);
                sheet::PitchDef pitchDef;
                pitchDef.pitch = pitch;
                pitchDef.octave = octave;
                ctx->renderPitch(pitchDef, duration, velocity, isTied);   
            }
            ctx->seek(-offset);
            lua_pop(L, 1);
        }

        void LuaModification::performResult(AContext *ctx, const Event &ev) {
            if (!lua_istable(L, -1)) {
                FM_THROW(Exception, "lua result is not a table");
            }
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) { // every events
                if (!lua_istable(L, -1)) {
                    lua_pop(L, 1);
                    continue;
                }
                
                PitchDef result;
                fm::String type;
                sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_TYPE, type);
                if (type == LUA_EVENT_TYPE_NOTE) {
                    performNoteResult(ctx);
                }
                lua_pop(L, 1);
            }
            ctx->seek(ev.duration);
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
            call(3, 1);
            performResult(ctx, ev);
        }
    }
}