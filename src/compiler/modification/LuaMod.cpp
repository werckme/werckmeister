#include "LuaMod.h"
#include <lua.hpp>
#include <lua/ALuaObject.h>
#include <com/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>
#include <compiler/lua/luaContext.h>
#include <compiler/context/IContext.h>
#include <compiler/context/MidiContext.h>
#include <lua/luaHelper.h>
#include <com/midi.hpp>
#include <com/common.hpp>

static const char *LUA_EVENT_TYPE_NOTE = "note";
static const char *LUA_EVENT_TYPE_REST = "rest";
static const char *LUA_EVENT_TYPE_DEGREE = "degree";
static const char *LUA_EVENT_TYPE_PITCHBEND = "pitchBend";
static const char *LUA_EVENT_TYPE_CC = "cc";
static const char *LUA_EVENT_TYPE_SYSEX = "sysex";
static const char *LUA_EVENT_TYPE_META = "meta";
static const char *LUA_EVENT_PROPERTY_SYSEX_DATA = "sysexData";
static const char *LUA_EVENT_TYPE_UNKNOWN = "unknown";
static const char *LUA_EVENT_PROPERTY_VELOCITY = "velocity";
static const char *LUA_EVENT_PROPERTY_DURATION = "duration";
static const char *LUA_EVENT_PROPERTY_TYING = "isTied";
static const char *LUA_EVENT_PROPERTY_OFFSET = "offset";
static const char *LUA_EVENT_PROPERTY_META_TYPE = "metaType";
static const char *LUA_EVENT_PROPERTY_META_STR_VALUE = "metaValue";
static const char *LUA_EVENT_PROPERTY_CC_NR = "ccNr";
static const char *LUA_EVENT_PROPERTY_CC_VALUE = "ccValue";
static const char *LUA_EVENT_PROPERTY_PITCHES = "pitches";
static const char *LUA_EVENT_PROPERTY_TYPE = "type";
static const char *LUA_EVENT_PROPERTY_PITCHBENDVALUE = "pitchBendValue";
static const char *LUA_EVENT_PITCH_PROPERTY_PITCH = "pitch";
static const char *LUA_EVENT_PITCH_PROPERTY_TAGS = "tags";
static const char *LUA_EVENT_PITCH_PROPERTY_OCTAVE = "octave";
static const char *LUA_EVENT_PITCH_PROPERTY_ALIAS = "alias";
static const char *LUA_EVENT_PROPERTY_TOAL_TIED_DURATION = "totalTiedDuration";
static const char *LUA_EVENT_PROPERTY_TIED_DURATION = "tiedDuration";
static const char* LUA_EVENT_PROPERTY_TRACK_ID = "trackId";
static const char* LUA_EVENT_PROPERTY_VOICE_ID = "voiceId";
static const char* LUA_EVENT_PROPERTY_PRIO = "prio";

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
        using namespace documentModel;
        lua_createtable(L, 2, 0);
        auto top = lua_gettop(L);
        // type
        lua::setTableValue(L, LUA_EVENT_PROPERTY_TYPE, top, getTypename());
        IContext::TrackId trackId = ctx->track();
        // track id
        lua::setTableValue(L, LUA_EVENT_PROPERTY_TRACK_ID, top, trackId);
        IContext::TrackId voiceId = ctx->voice();
        // voice id
        lua::setTableValue(L, LUA_EVENT_PROPERTY_VOICE_ID, top, voiceId);
        // tags
        lua_pushstring(L, LUA_EVENT_PITCH_PROPERTY_TAGS);
        pushTags(L);
        lua_settable(L, top);
        // offset
        lua::setTableValue(L, LUA_EVENT_PROPERTY_OFFSET, top, event->offset / com::PPQ);
        switch (event->type)
        {
            case Event::Controller:
                lua::setTableValue(L, LUA_EVENT_PROPERTY_CC_NR, top, event->controllerNumber);
                lua::setTableValue(L, LUA_EVENT_PROPERTY_CC_VALUE, top, event->controllerValue);
                break;
            default:
                // pitches
                lua_pushstring(L, LUA_EVENT_PROPERTY_PITCHES);
                pushPitches(L);
                lua_settable(L, top);
                // velocity
                lua::setTableValue(L, LUA_EVENT_PROPERTY_VELOCITY, top, event->velocity);
                // duration
                lua::setTableValue(L, LUA_EVENT_PROPERTY_DURATION, top, event->duration / com::PPQ);
                // is tied
                lua::setTableValue(L, LUA_EVENT_PROPERTY_TYING, top, event->isTied());
                // pitchbend value
                pushPitchBendValue(L, top, *event);
                // totalTiedDuration
                lua::setTableValue(L, LUA_EVENT_PROPERTY_TOAL_TIED_DURATION, top, event->tiedDurationTotal / com::PPQ);
                // tiedDuration
                lua::setTableValue(L, LUA_EVENT_PROPERTY_TIED_DURATION, top, event->tiedDuration / com::PPQ);
        }
    }

    void LuaEvent::pushPitchBendValue(lua_State *L, int top, const documentModel::Event &event)
    {
        if (event.type != documentModel::Event::PitchBend)
        {
            return;
        }
        lua::setTableValue(L, LUA_EVENT_PROPERTY_PITCHBENDVALUE, top, event.pitchBendValue);
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
            lua_pushstring(L, LUA_EVENT_PITCH_PROPERTY_PITCH);
            lua_pushnumber(L, resolved.pitch);
            lua_settable(L, objecttop);
            // octave
            lua_pushstring(L, LUA_EVENT_PITCH_PROPERTY_OCTAVE);
            lua_pushnumber(L, resolved.octave);
            lua_settable(L, objecttop);
            // alias
            lua_pushstring(L, LUA_EVENT_PITCH_PROPERTY_ALIAS);
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
        case Event::Rest:
            return LUA_EVENT_TYPE_REST;
        case Event::Controller:
            return LUA_EVENT_TYPE_CC;
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
        lua::getTableValue(L, LUA_EVENT_PROPERTY_DURATION, event.duration);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_OFFSET, event.offset);
        event.duration *= com::PPQ;
        event.offset *= com::PPQ;
        if (event.type == documentModel::Event::Rest) 
        {
            return;
        }
        lua::getTableValue(L, LUA_EVENT_PROPERTY_VELOCITY, event.velocity);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_TOAL_TIED_DURATION, event.tiedDurationTotal);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_TIED_DURATION, event.tiedDuration);
        event.tiedDuration *= com::PPQ;
        event.tiedDurationTotal *= com::PPQ;
        bool isTied = false;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_TYING, isTied);
        event.isTied(isTied);
        lua_pushstring(L, LUA_EVENT_PROPERTY_PITCHES);
        lua_gettable(L, -2);
        if (!lua_istable(L, -1))
        {
            FM_THROW(Exception, "missing pitches");
        }
        lua_pushnil(L);
        while (lua_next(L, -2) != 0)
        {
            int pitch = 0;
            lua::getTableValue(L, LUA_EVENT_PITCH_PROPERTY_PITCH, pitch);
            int octave = 0;
            lua::getTableValue(L, LUA_EVENT_PITCH_PROPERTY_OCTAVE, octave);
            lua_pop(L, 1);
            documentModel::PitchDef pitchDef;
            pitchDef.pitch = pitch;
            pitchDef.octave = octave;
            event.pitches.emplace_back(pitchDef);
        }
        lua_pop(L, 1);
        // get tags
        lua_pushstring(L, LUA_EVENT_PITCH_PROPERTY_TAGS);
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
        lua::getTableValue(L, LUA_EVENT_PROPERTY_PITCHBENDVALUE, event.pitchBendValue);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_OFFSET, event.offset);
        event.offset *= com::PPQ;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_DURATION, event.duration);
    }

    AModification::Events LuaModification::popEvents(IContextPtr ctx)
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
            lua::getTableValue(L, LUA_EVENT_PROPERTY_TYPE, type);
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
            if (type == LUA_EVENT_TYPE_REST)
            {
                Event event;
                event.type = Event::Rest;
                popNoteEvent(event);
                result.push_back(event);
            }
            if(type == LUA_EVENT_TYPE_CC)
            {
                popAndExecuteCc(ctx);
            }
            if (type == LUA_EVENT_TYPE_SYSEX)
            {
                popAndExecuteSysex(ctx);
            }
            if (type == LUA_EVENT_TYPE_META)
            {
                popAndExecuteMeta(ctx);
            }
            lua_pop(L, 1);
        }
        return result;
    }

    void LuaModification::popAndExecuteCc(IContextPtr ctx)
    {
        com::Ticks timeOffset = 0;
        int ccNr = -1;
        int ccValue = -1;
        int prio = -1;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_OFFSET, timeOffset);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_CC_NR, ccNr);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_CC_VALUE, ccValue);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_PRIO, prio, false);
        timeOffset *= com::PPQ;
        ctx->setContinuousController(ccNr, ccValue, timeOffset, prio);
    }

    namespace 
    {
        const std::map<com::String, com::midi::MetaEventType> _metaTypes({
            {"textEvent", com::midi::TextEvent},
            {"copyright", com::midi::Copyright},
            {"sequenceOrTrackName", com::midi::SequenceOrTrackName},
            {"instrumentName", com::midi::InstrumentName},
            {"lyricText", com::midi::LyricText},
            {"markerText", com::midi::MarkerText},
            {"cuePoint", com::midi::CuePoint},
        });
    }

    void LuaModification::popAndExecuteMeta(IContextPtr ctx)
    {
        auto midiContext = std::dynamic_pointer_cast<MidiContext>(ctx);
        if (!midiContext)
        {
            FM_THROW(Exception, "meta event not supported in non midi environment");
        }
        com::String type;
        com::String value;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_META_TYPE, type);
        auto itMetaType = _metaTypes.find(type);
        if (itMetaType == _metaTypes.end())
        {
            com::StringStream ss;
            ss << "unsupported midi type: " << type << std::endl;
            ss << "supported types are: ";
            for(const auto& supportedType : _metaTypes)
            {
                ss << supportedType.first << " ";
            }
            FM_THROW(Exception, ss.str());
        }
        lua::getTableValue(L, LUA_EVENT_PROPERTY_META_STR_VALUE, value);
        auto ev = com::midi::Event();
        auto bytes = com::midi::Event::MetaCreateStringData(value);
        ev.metaData(itMetaType->second, bytes.data(), bytes.size());
        auto pos = ctx->currentPosition();
        com::Ticks offset = 0;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_OFFSET, offset, false);
        ev.absPosition(pos + offset * com::PPQ);


		auto visitor = ctx->compilerVisitor();
		auto trackMeta = ctx->trackMetaData();
        if (trackMeta)
        {
            // #450: cue event had no insrument informations, because it was 
            // the first event in a voice.
            // A better fix may be, handle midi events as regular events and not here instantly, 
            // so that the event process chain will be walked through properly
            visitor->visitInstrument(trackMeta->uname, trackMeta->uname);
        }
        midiContext->addEvent(ev);
    }

    void LuaModification::popAndExecuteSysex(IContextPtr ctx)
    {
        com::Ticks timeOffset = 0;
        int prio = -1;
        lua::getTableValue(L, LUA_EVENT_PROPERTY_OFFSET, timeOffset);
        lua::getTableValue(L, LUA_EVENT_PROPERTY_PRIO, prio, false);
        timeOffset *= com::PPQ;
        lua_pushstring(L, LUA_EVENT_PROPERTY_SYSEX_DATA);
        lua_gettable(L, -2);
        if (!lua_istable(L, -1))
        {
            FM_THROW(Exception, "sysex data is not a table");
        }
        size_t size = lua_rawlen(L, -1);
        std::vector<com::Byte> bff;
        bff.reserve(size);
        for (size_t index = 0; index < size; index++) 
        {
            lua_pushinteger(L, index + 1);
            lua_gettable(L, -2);
            if (!lua_isinteger(L, -1))
            {
                FM_THROW(Exception, "sysex data expecting number value");
            }
            int value = lua_tointeger(L, -1);
            if (value < 0 || value > 255)
            {
                FM_THROW(Exception, "sysex value out of bounds: " + std::to_string(value));
            }
            com::Byte byteValue = (com::Byte)value;
            bool isSyexEscapeByte = index == 0 && byteValue == 0xF0;
            isSyexEscapeByte |= index == size -1 && byteValue == 0xF7;
            if (!isSyexEscapeByte) // will be added later
            {
                bff.push_back(byteValue);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
        if (bff.empty())
        {
            return;
        }
        ctx->setSysex(bff.data(), bff.size(), timeOffset, prio);
    }

    void LuaModification::perform(IContextPtr ctx, Events &events)
    {
        lua_getglobal(L, LUA_MODIFICATION_FENTRY);
        pushEvents(ctx, events);
        pushParameters(L, ALuaWithParameter::parameters);
        lua::LuaTimeInfo ltimeInfo(ctx->getTimeInfo());
        ltimeInfo.push(L);
        lua::LuaContext luaCtx(*ctx.get());
        luaCtx.push(L);
        call(4, 1);
        auto processedEvents = popEvents(ctx);
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