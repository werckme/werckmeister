#include "Lua.h"
#include <lua.hpp>
#include <lua/ALuaObject.h>
#include <com/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>

namespace compiler
{
    namespace luaChord
    {
        struct LuaChord : lua::ALuaObject
        {
            typedef lua::ALuaObject Base;
            const documentModel::ChordDef *chordDef;
            const documentModel::Event *chordEvent;
            LuaChord(const documentModel::ChordDef *chordDef, const documentModel::Event *chordEvent)
                : chordDef(chordDef), chordEvent(chordEvent)
            {
            }
            void push(lua_State *L);
            void pushChordName(lua_State *L);
            void pushChordDegrees(lua_State *L);
            void pushChordDegree(lua_State *L, const documentModel::DegreeDef &DegreeDef);
        };
        void LuaChord::pushChordName(lua_State *L)
        {
            auto chordElements = chordEvent->chordElements();
            auto base = std::get<0>(chordElements);
            auto options = std::get<1>(chordElements);
            auto strBase = com::pitchToString(base);
            int top = lua_gettop(L);
            lua_pushstring(L, "strOptions");
            lua_pushstring(L, options.c_str());
            lua_settable(L, top);
            top = lua_gettop(L);
            lua_pushstring(L, "strBase");
            lua_pushstring(L, strBase.c_str());
            lua_settable(L, top);
            top = lua_gettop(L);
            lua_pushstring(L, "rootPitch");
            lua_pushinteger(L, base);
            lua_settable(L, top);
        }
        void LuaChord::pushChordDegree(lua_State *L, const documentModel::DegreeDef &DegreeDef)
        {
            auto top = lua_gettop(L);
            lua_pushinteger(L, DegreeDef.degree);
            lua_pushinteger(L, DegreeDef.value);
            lua_settable(L, top);
        }
        void LuaChord::pushChordDegrees(lua_State *L)
        {
            auto top = lua_gettop(L);
            lua_pushstring(L, "degrees");
            lua_createtable(L, chordDef->degreeDefs.size(), 0);
            for (const auto &degreeDef : chordDef->degreeDefs)
            {
                pushChordDegree(L, degreeDef);
            }
            lua_settable(L, top);
        }

        template <int Degree>
        bool hasDegreeImpl(const documentModel::ChordDef &def)
        {
            return false;
        }
        template <>
        bool hasDegreeImpl<7>(const documentModel::ChordDef &def)
        {
            return has7(def);
        }
        template <>
        bool hasDegreeImpl<9>(const documentModel::ChordDef &def)
        {
            return has9(def);
        }
        template <>
        bool hasDegreeImpl<11>(const documentModel::ChordDef &def)
        {
            return has11(def);
        }
        template <>
        bool hasDegreeImpl<13>(const documentModel::ChordDef &def)
        {
            return has13(def);
        }
        template <int Degree>
        static int luaHas7(lua_State *L)
        {
            using namespace lua;
            auto luaChord = ALuaObject::getObject<LuaChord>(L, -1);
            lua_pushboolean(L, hasDegreeImpl<Degree>(*(luaChord->chordDef)));
            return 1;
        }

        static int luaIsDegreeAdjunct(lua_State* L)
        {
            using namespace lua;
            int degree = lua_tointeger(L, -1);
            auto luaChord = ALuaObject::getObject<LuaChord>(L, -2);
            const auto *degreeDef = luaChord->chordDef->findDegreeDef(degree);
            if (degreeDef == nullptr) 
            {
                return false;
            }
            lua_pushboolean(L, degreeDef->isAdjunct);
            return 1;
        }

        static const luaL_Reg libfs[] = {
            {"has7", luaHas7<7>},
            {"has9", luaHas7<9>},
            {"has11", luaHas7<11>},
            {"has13", luaHas7<13>},
            {"isDegreeAdjunct", luaIsDegreeAdjunct},
            {NULL, NULL}};

        void LuaChord::push(lua_State *L)
        {
            Base::push(L, libfs, 1);
            pushChordName(L);
            pushChordDegrees(L);
        }
    }
    namespace luaPitches
    {
        const char *LuaPitchKeyPitch = "pitch";
        const char *LuaPitchKeyOctave = "octave";
        const char *LuaPitchKeyRoot = "root";
        const char *LuaPitchKeyDegreeValue = "degreeValue";
        const char* LuaPitchKeyIsForced = "isForced";
        enum
        {
            NoDegreeValue = INT_MAX
        };
        struct LuaPitches : lua::ALuaObject
        {
            typedef lua::ALuaObject Base;
            const documentModel::ChordDef *chordDef;
            const documentModel::Event *chordEvent;
            const LuaVoicingStrategy::Pitches *degrees;
            LuaPitches(const documentModel::ChordDef *chordDef, const documentModel::Event *chordEvent, const VoicingStrategy::Pitches *degrees)
                : chordDef(chordDef), chordEvent(chordEvent), degrees(degrees)
            {
            }
            void push(lua_State *L);
            void pushDegrees(lua_State *L);
            void pushDegrees(lua_State *L, documentModel::PitchDef::Pitch root, int degreeValue, const std::vector<documentModel::PitchDef> &degrees);
            void pushDegree(lua_State *L, documentModel::PitchDef::Pitch root, int degreeValue, documentModel::PitchDef::Octave octave, bool isForced);
        };

        void LuaPitches::pushDegree(lua_State *L, documentModel::PitchDef::Pitch root, int degreeValue, documentModel::PitchDef::Octave octave, bool isForced)
        {
            lua_createtable(L, 2, 0);
            auto top = lua_gettop(L);
            lua_pushstring(L, LuaPitchKeyOctave);
            lua_pushinteger(L, octave);
            lua_settable(L, top);
            if (degreeValue == NoDegreeValue)
            {
                return;
            }
            top = lua_gettop(L);
            lua_pushstring(L, LuaPitchKeyDegreeValue);
            lua_pushinteger(L, degreeValue);
            lua_settable(L, top);
            top = lua_gettop(L);
            lua_pushstring(L, LuaPitchKeyIsForced);
            lua_pushboolean(L, isForced);
            lua_settable(L, top);
        }

        void LuaPitches::pushDegrees(lua_State *L)
        {
            using namespace documentModel;
            if (degrees->empty())
            {
                return;
            }
            auto chordElements = chordEvent->chordElements();
            auto root = std::get<0>(chordElements);
            std::vector<PitchDef> orderedByPitch(degrees->begin(), degrees->end());
            auto degreeComp = [](const PitchDef &a, const PitchDef &b)
            { return getDegreeValue(a.pitch) < getDegreeValue(b.pitch); };
            std::sort(orderedByPitch.begin(), orderedByPitch.end(), degreeComp);
            auto it = orderedByPitch.begin();
            while (it < orderedByPitch.end())
            {
                // <I I'> => I=[{I}, {I'}]
                // II => II=[{II}]
                auto degreeValue = getDegreeValue(it->pitch);
                auto grouped = std::equal_range(orderedByPitch.begin(), orderedByPitch.end(), *it, degreeComp);
                std::vector<PitchDef> degreesRanged(grouped.first, grouped.second);
                pushDegrees(L, root, degreeValue, degreesRanged);
                it = grouped.second; // it++
            }
        }

        void LuaPitches::pushDegrees(lua_State *L, documentModel::PitchDef::Pitch root, int degreeValue, const std::vector<documentModel::PitchDef> &degrees_)
        {
            auto luaStackMainTable = lua_gettop(L);
            lua_pushinteger(L, degreeValue);
            lua_createtable(L, degrees_.size(), 0);
            auto luaStackDegrees = lua_gettop(L);
            int degreeIndex = 1;
            for (const auto &degree : degrees_)
            {
                auto degreeDef = chordDef->resolveDegreeDef(degree);
                if (!degreeDef.valid())
                {
                    degreeDef.value = NoDegreeValue;
                }
                lua_pushinteger(L, degreeIndex++);
                pushDegree(L, root, degreeDef.value, degree.octave, degree.forceDegree);
                lua_settable(L, luaStackDegrees);
            }
            lua_settable(L, luaStackMainTable);
        }

        void LuaPitches::push(lua_State *L)
        {
            Base::push(L, NULL, 0);
            pushDegrees(L);
        }
    }
}

namespace compiler
{
    LuaVoicingStrategy::LuaVoicingStrategy(const com::String &path) : LuaBase(path)
    {
    }

    bool LuaVoicingStrategy::canExecute() const
    {
        return LuaBase::hasFunction(LUA_VOICING_STRATEGY_FENTRY);
    }

    void LuaVoicingStrategy::assertCanExecute() const
    {
        if (!canExecute())
        {
            FM_THROW(Exception, com::String("missing '") + LUA_VOICING_STRATEGY_FENTRY + "' function");
        }
    }

    documentModel::PitchDef LuaVoicingStrategy::popPitch(lua_State *L)
    {
        documentModel::PitchDef result;
        lua_pushstring(L, luaPitches::LuaPitchKeyOctave);
        lua_gettable(L, -2);
        result.octave = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_pushstring(L, luaPitches::LuaPitchKeyPitch);
        lua_gettable(L, -2);
        result.pitch = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return result;
    }

    LuaVoicingStrategy::Pitches LuaVoicingStrategy::popPitches(lua_State *L)
    {
        LuaVoicingStrategy::Pitches result;
        if (!lua_istable(L, -1))
        {
            FM_THROW(Exception, "lua result is not a table");
        }
        lua_pushnil(L);
        while (lua_next(L, -2) != 0)
        {
            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }
            result.push_back(popPitch(L));
            lua_pop(L, 1);
        }
        return result;
    }

    LuaVoicingStrategy::Pitches LuaVoicingStrategy::solve(const documentModel::Event &chord,
                                                        const documentModel::ChordDef &def,
                                                        const Pitches &degreePitches,
                                                        const TimeInfo &t)
    {
        try
        {
            lua_getglobal(L, LUA_VOICING_STRATEGY_FENTRY);
            luaChord::LuaChord luaChord(&def, &chord);
            luaChord.push(L);
            luaPitches::LuaPitches luaPitches(&def, &chord, &degreePitches);
            luaPitches.push(L);
            pushParameters(L, ALuaWithParameter::parameters);
            lua::LuaTimeInfo(t).push(L);
            call(4, 1);
            return popPitches(L);
        }
        catch (const std::exception &ex)
        {
            com::StringStream ss;
            ss << "failed to process lua script: '" << path() << "'" << std::endl;
            ss << "failed to execute voicing function: '" << LUA_VOICING_STRATEGY_FENTRY << "'" << std::endl;
            ss << ex.what();
            FM_THROW(Exception, ss.str());
        }
    }

    LuaVoicingStrategy::ParametersByNames &LuaVoicingStrategy::getParameters()
    {
        return lua::ALuaWithParameter::getParameters(L);
    }
}