#include "LuaMod.h"
#include <lua.hpp>
#include <lua/ALuaObject.h>
#include <com/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/context/IContext.h>
#include <lua/luaHelper.h>

static const char *LUA_EVENT_TYPE_NOTE = "note";
static const char *LUA_EVENT_TYPE_DEGREE = "degree";
static const char *LUA_EVENT_TYPE_PITCHBEND = "pitchBend";
static const char *LUA_EVENT_TYPE_UNKNOWN = "unknown";
static const char *LUA_EVENT_PROPETRY_VELOCITY = "velocity";
static const char *LUA_EVENT_PROPETRY_DURATION = "duration";
static const char *LUA_EVENT_PROPETRY_TYING = "isTied";
static const char *LUA_EVENT_PROPETRY_OFFSET = "offset";
static const char *LUA_EVENT_PROPETRY_PITCHES = "pitches";
static const char *LUA_EVENT_PROPETRY_TYPE = "type";
static const char *LUA_EVENT_PROPETRY_PITCHBENDVALUE = "pitchBendValue";
static const char *LUA_EVENT_PITCH_PROPETRY_PITCH = "pitch";
static const char *LUA_EVENT_PITCH_PROPETRY_TAGS = "tags";
static const char *LUA_EVENT_PITCH_PROPETRY_OCTAVE = "octave";
static const char *LUA_EVENT_PITCH_PROPETRY_ALIAS = "alias";
static const char *LUA_EVENT_PROPETRY_TOAL_TIED_DURATION = "totalTiedDuration";
static const char *LUA_EVENT_PROPERTY_TIED_DURATION = "tiedDuration";
static const char* LUA_EVENT_PROPERTY_TRACK_ID = "trackId";
static const char* LUA_EVENT_PROPERTY_VOICE_ID = "voiceId";

namespace compiler
{
    struct LuaEvent : lua::ALuaObject
    {
        typedef lua::ALuaObject Base;
        const documentModel::Event *event;
        IContextPtr ctx;
        LuaEvent(const documentModel::Event *event, IContextPtr ctx)
            : event(event), ctx(ctx)
        {
        }
        void push(lua_State *L);
        void pushPitches(lua_State *L);
        void pushTags(lua_State *L);
        void pushPitchBendValue(lua_State *L, int top, const documentModel::Event &event);
        const char *getTypename() const;
    };
    void LuaEvent::push(lua_State *L)
    {
        lua_createtable(L, 2, 0);
        auto top = lua_gettop(L);
        // type
        lua::setTableValue(L, LUA_EVENT_PROPETRY_TYPE, top, getTypename());
        // pitches
        lua_pushstring(L, LUA_EVENT_PROPETRY_PITCHES);
        pushPitches(L);
        lua_settable(L, top);
        // tags
        lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_TAGS);
        pushTags(L);
        lua_settable(L, top);
        // offset
        lua::setTableValue(L, LUA_EVENT_PROPETRY_OFFSET, top, event->offset / com::PPQ);
        // velocity
        lua::setTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, top, event->velocity);
        // duration
        lua::setTableValue(L, LUA_EVENT_PROPETRY_DURATION, top, event->duration / com::PPQ);
        // is tied
        lua::setTableValue(L, LUA_EVENT_PROPETRY_TYING, top, event->isTied());
        // pitchbend value
        pushPitchBendValue(L, top, *event);
        // totalTiedDuration
        lua::setTableValue(L, LUA_EVENT_PROPETRY_TOAL_TIED_DURATION, top, event->tiedDurationTotal / com::PPQ);
        // tiedDuration
        lua::setTableValue(L, LUA_EVENT_PROPERTY_TIED_DURATION, top, event->tiedDuration / com::PPQ);
        IContext::TrackId trackId = ctx->track();
        // track id
        lua::setTableValue(L, LUA_EVENT_PROPERTY_TRACK_ID, top, trackId);
        IContext::TrackId voiceId = ctx->voice();
        // voice id
        lua::setTableValue(L, LUA_EVENT_PROPERTY_VOICE_ID, top, voiceId);
    }

    void LuaEvent::pushPitchBendValue(lua_State *L, int top, const documentModel::Event &event)
    {
        if (event.type != documentModel::Event::PitchBend)
        {
            return;
        }
        lua::setTableValue(L, LUA_EVENT_PROPETRY_PITCHBENDVALUE, top, event.pitchBendValue);
    }

    void LuaEvent::pushPitches(lua_State *L)
    {
        lua_createtable(L, event->pitches.size(), 0);
        auto top = lua_gettop(L);
        int count = 1;
        for (const auto &pitch : event->pitches)
        {
            lua_pushnumber(L, count++);
            lua_createtable(L, event->pitches.size(), 0);
            auto objecttop = lua_gettop(L);
            auto resolved = ctx->definitionsServer()->resolvePitch(pitch);
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
        for (const auto &tag : event->tags)
        {
            lua_pushnumber(L, count++);
            lua_pushstring(L, tag.c_str());
            lua_settable(L, top);
        }
    }
    const char *LuaEvent::getTypename() const
    {
        using namespace documentModel;
        switch (event->type)
        {
        case Event::Note:
            return LUA_EVENT_TYPE_NOTE;
        case Event::TiedNote:
            return LUA_EVENT_TYPE_NOTE;
        case Event::Degree:
            return LUA_EVENT_TYPE_DEGREE;
        case Event::TiedDegree:
            return LUA_EVENT_TYPE_DEGREE;
        case Event::PitchBend:
            return LUA_EVENT_TYPE_PITCHBEND;
        default:
            return LUA_EVENT_TYPE_UNKNOWN;
        }
        return nullptr;
    }
}

namespace compiler
{
    LuaModification::LuaModification(const com::String &path) : LuaBase(path)
    {
    }

    bool LuaModification::canExecute() const
    {
        return LuaBase::hasFunction(LUA_MODIFICATION_FENTRY);
    }

    void LuaModification::assertCanExecute() const
    {
        if (!canExecute())
        {
            FM_THROW(Exception, com::String("missing '") + LUA_MODIFICATION_FENTRY + "' function");
        }
    }

    void LuaModification::popNoteEvent(documentModel::Event &event)
    {
        lua::getTableValue(L, LUA_EVENT_PROPETRY_VELOCITY, event.velocity);
        lua::getTableValue(L, LUA_EVENT_PROPETRY_OFFSET, event.offset);
        lua::getTableValue(L, LUA_EVENT_PROPETRY_TOAL_TIED_DURATION, event.tiedDurationTotal);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_TIED_DURATION, event.tiedDuration);
        event.offset *= com::PPQ;
        event.tiedDuration *= com::PPQ;
        event.tiedDurationTotal *= com::PPQ;
        lua::getTableValue(L, LUA_EVENT_PROPETRY_DURATION, event.duration);
        bool isTied = false;
        lua::getTableValue(L, LUA_EVENT_PROPETRY_TYING, isTied);
        event.isTied(isTied);
        event.duration *= com::PPQ;
        lua_pushstring(L, LUA_EVENT_PROPETRY_PITCHES);
        lua_gettable(L, -2);
        if (!lua_istable(L, -1))
        {
            FM_THROW(Exception, "missing pitches");
        }
        lua_pushnil(L);
        while (lua_next(L, -2) != 0)
        {
            int pitch = 0;
            lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_PITCH, pitch);
            int octave = 0;
            lua::getTableValue(L, LUA_EVENT_PITCH_PROPETRY_OCTAVE, octave);
            lua_pop(L, 1);
            documentModel::PitchDef pitchDef;
            pitchDef.pitch = pitch;
            pitchDef.octave = octave;
            event.pitches.emplace_back(pitchDef);
        }
        lua_pop(L, 1);
        // get tags
        lua_pushstring(L, LUA_EVENT_PITCH_PROPETRY_TAGS);
        lua_gettable(L, -2);
        if (!lua_istable(L, -1))
        {
            lua_pop(L, 1);
            return;
        }
        lua_pushnil(L);
        while (lua_next(L, -2) != 0)
        {
            std::string metaValue(lua_tostring(L, -1));
            event.tags.emplace(metaValue);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }

    void LuaModification::popPitchBendEvent(documentModel::Event &event)
    {
        lua::getTableValue(L, LUA_EVENT_PROPETRY_PITCHBENDVALUE, event.pitchBendValue);
        lua::getTableValue(L, LUA_EVENT_PROPETRY_OFFSET, event.offset);
        event.offset *= com::PPQ;
        lua::getTableValue(L, LUA_EVENT_PROPETRY_DURATION, event.duration);
    }

    AModification::Events LuaModification::popEvents()
    {
        using namespace documentModel;
        Events result;
        if (!lua_istable(L, -1))
        {
            FM_THROW(Exception, "lua result is not a table");
        }
        lua_pushnil(L);
        while (lua_next(L, -2) != 0)
        { // every events
            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }
            com::String type;
            lua::getTableValue(L, LUA_EVENT_PROPETRY_TYPE, type);
            if (type == LUA_EVENT_TYPE_NOTE || type == LUA_EVENT_TYPE_DEGREE)
            {
                Event event;
                event.type = (type == LUA_EVENT_TYPE_NOTE) ? Event::Note : Event::Degree;
                popNoteEvent(event);
                result.push_back(event);
            }
            if (type == LUA_EVENT_TYPE_PITCHBEND)
            {
                Event event;
                event.type = Event::PitchBend;
                popPitchBendEvent(event);
                result.push_back(event);
            }
            lua_pop(L, 1);
        }
        return result;
    }

    void LuaModification::perform(IContextPtr ctx, Events &events)
    {
        lua_getglobal(L, LUA_MODIFICATION_FENTRY);
        pushEvents(ctx, events);
        pushParameters(L, ALuaWithParameter::parameters);
        lua::LuaTimeInfo(ctx->getTimeInfo()).push(L);
        call(3, 1);
        auto processedEvents = popEvents();
        events.swap(processedEvents);
    }

    void LuaModification::pushEvents(IContextPtr ctx, const Events &events)
    {
        lua_createtable(L, events.size(), 0);
        auto top = lua_gettop(L);
        int luaTableIndex = 1;
        for (const auto &event : events)
        {
            lua_pushinteger(L, luaTableIndex++);
            LuaEvent(&event, ctx).push(L);
            lua_settable(L, top);
        }
    }

    LuaModification::ParametersByNames &LuaModification::getParameters()
    {
        return lua::ALuaWithParameter::getParameters(L);
    }
}