#include "LuaMod.h"
#include <lua.hpp>
#include <fm/lua/ALuaObject.h>
#include <fm/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/context/AContext.h>
#include <fm/lua/luaHelper.h>

static const char * LUA_EVENT_TYPE_NOTE = "note";
static const char * LUA_EVENT_TYPE_DEGREE = "degree";
static const char * LUA_EVENT_TYPE_PITCHBEND = "pitchBend";
static const char * LUA_EVENT_TYPE_UNKNOWN = "unknown";
static const char * LUA_EVENT_PROPETRY_VELOCITY = "velocity";
static const char * LUA_EVENT_PROPETRY_DURATION = "duration";
static const char * LUA_EVENT_PROPETRY_TYING = "isTied";
static const char * LUA_EVENT_PROPETRY_OFFSET = "offset";
static const char * LUA_EVENT_PROPETRY_PITCHES = "pitches";
static const char * LUA_EVENT_PROPETRY_TYPE = "type";
static const char * LUA_EVENT_PROPETRY_PITCHBENDVALUE = "pitchBendValue";
static const char * LUA_EVENT_PITCH_PROPETRY_PITCH = "pitch";
static const char * LUA_EVENT_PITCH_PROPETRY_TAGS = "tags";
static const char * LUA_EVENT_PITCH_PROPETRY_OCTAVE = "octave";
static const char * LUA_EVENT_PITCH_PROPETRY_ALIAS = "alias";

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
			void pushTags(lua_State *L);
            void pushPitchBendValue(lua_State *L, int top, const Event &event);
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
			// tags
			lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_TAGS);
			pushTags(L);
			lua_settable(L, top);
            // offset
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_OFFSET, top, event->offset / fm::PPQ);            
            // velocity
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, top, event->velocity);      
            // duration
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_DURATION, top, event->duration / fm::PPQ);
            // is tied
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_TYING, top, event->isTied());
            // pitchbend value
            pushPitchBendValue(L, top, *event);
                             
        }

        void LuaEvent::pushPitchBendValue(lua_State *L, int top, const Event &event)
        {
            if (event.type != Event::PitchBend) {
                return;
            }
            sheet::lua::setTableValue(L, LUA_EVENT_PROPETRY_PITCHBENDVALUE, top, event.pitchBendValue);
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
                auto resolved = ctx->resolvePitch(pitch);
                // pitch
                lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_PITCH);
                lua_pushnumber(L, resolved.pitch);
                lua_settable(L, objecttop);
                // octave
                lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_OCTAVE);
                lua_pushnumber(L, resolved.octave);
                lua_settable(L, objecttop);  
                // alias
                lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_ALIAS);
                lua_pushstring(L, pitch.alias.c_str());
                lua_settable(L, objecttop);  
                // iterate
                lua_settable(L, top);
            }
        }
		void LuaEvent::pushTags(lua_State *L)
		{
			lua_createtable(L, event->tags.size(), 0);
			auto top = lua_gettop(L);
			int count = 1;
			for (const auto &tag : event->tags) {
				lua_pushnumber(L, count++);
				lua_pushstring(L, tag.c_str());
				lua_settable(L, top);
			}
		}
        const char * LuaEvent::getTypename() const
        {
            switch (event->type)
            {
            case Event::TiedNote: return LUA_EVENT_TYPE_DEGREE;
            case Event::Note: return LUA_EVENT_TYPE_NOTE;
            case Event::PitchBend: return LUA_EVENT_TYPE_PITCHBEND;
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

        void LuaModification::popNoteEvent(Event &event)
        {
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, event.velocity);
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_OFFSET, event.offset);
            event.offset *= fm::PPQ;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_DURATION, event.duration);
            bool isTied = false;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_TYING, isTied);
            event.isTied(isTied);
            event.duration *= fm::PPQ;            
            lua_pushstring(L, LUA_EVENT_PROPETRY_PITCHES);
            lua_gettable(L, -2);
            if (!lua_istable(L, -1)) {
                FM_THROW(Exception, "missing pitches");
            }
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                int pitch = 0;
                sheet::lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_PITCH, pitch);
                int octave = 0;
                sheet::lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_OCTAVE, octave);
                lua_pop(L, 1);
                sheet::PitchDef pitchDef;
                pitchDef.pitch = pitch;
                pitchDef.octave = octave;
                event.pitches.emplace_back(pitchDef);
            }
            lua_pop(L, 1);
            // get tags
            lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_TAGS);
            lua_gettable(L, -2);
            if (!lua_istable(L, -1)) {
                lua_pop(L, 1);
                return;
            }
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                std::string metaValue(lua_tostring(L, -1));
                event.tags.emplace(metaValue);
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }

        void LuaModification::popPitchBendEvent(Event &event)
        {
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_PITCHBENDVALUE, event.pitchBendValue);
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_OFFSET, event.offset);
            event.offset *= fm::PPQ;
            sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_DURATION, event.duration);
        }


        AModification::Events LuaModification::popEvents() 
        {
            Events result;
            if (!lua_istable(L, -1)) {
                FM_THROW(Exception, "lua result is not a table");
            }
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) { // every events
                if (!lua_istable(L, -1)) {
                    lua_pop(L, 1);
                    continue;
                }
                fm::String type;
                sheet::lua::getTableValue(L, LUA_EVENT_PROPETRY_TYPE, type);
                if (type == LUA_EVENT_TYPE_NOTE || type == LUA_EVENT_TYPE_DEGREE) {
                    Event event;
                    event.type = (type == LUA_EVENT_TYPE_NOTE) ? Event::Note : Event::Degree;
                    popNoteEvent(event);
                    result.push_back(event);
                }
                if (type == LUA_EVENT_TYPE_PITCHBEND) {
                    Event event;
                    event.type = Event::PitchBend;
                    popPitchBendEvent(event);
                    result.push_back(event);
                }                
                lua_pop(L, 1);
            }
            return result;
        }

        void LuaModification::perform(AContext *ctx, Events &events)
        {
            lua_getglobal(L, LUA_MODIFICATION_FENTRY);
            pushEvents(ctx, events);
            pushParameters(L, ALuaWithParameter::parameters);
            lua::LuaTimeInfo(ctx->getTimeInfo()).push(L);
            call(3, 1);
            auto processedEvents = popEvents();
            events.swap(processedEvents);
        }

        void LuaModification::pushEvents(AContext *ctx, const Events &events)
        {
            lua_createtable(L, events.size(), 0);
            auto top = lua_gettop(L);
            int luaTableIndex = 1;
            for (const auto &event : events) {
                lua_pushinteger(L, luaTableIndex++);
                LuaEvent(&event, ctx).push(L);
                lua_settable(L, top);
            }
        }

        LuaModification::ParametersByNames & LuaModification::getParameters()
        {
            return fm::lua::ALuaWithParameter::getParameters(L);
        }
    }
}