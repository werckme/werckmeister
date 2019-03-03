#include "Lua.h"
#include <lua.hpp>
#include <sheet/lua/ALuaObject.h>
#include <sheet/tools.h>

namespace sheet {
    namespace compiler {
        namespace luaChord {
            struct LuaChord : lua::ALuaObject {
                typedef lua::ALuaObject Base;
                const ChordDef *chordDef;
                const Event *chordEvent;
                LuaChord(const ChordDef *chordDef,const Event *chordEvent)
                    : chordDef(chordDef),  chordEvent(chordEvent)
                {}
                void push(lua_State *L);
                void pushChordName(lua_State *L);
                void pushChordDegrees(lua_State *L);
                void pushChordDegree(lua_State *L, const ChordOption &chordOption);
            };
            void LuaChord::pushChordName(lua_State *L)
            {
                auto chordElements = chordEvent->chordElements();
                auto base = std::get<0>(chordElements);
                auto options = fm::to_string(std::get<1>(chordElements));
                auto strBase = fm::to_string(pitchToString(base));
                int top = lua_gettop(L);
                lua_pushstring(L, "strOptions");
                lua_pushstring(L, options.c_str());
                lua_settable(L, top);
                top = lua_gettop(L);
                lua_pushstring(L, "strBase");
                lua_pushstring(L, strBase.c_str());
                lua_settable(L, top);
            }
            void LuaChord::pushChordDegree(lua_State *L, const ChordOption &chordOption)
            {
                lua_createtable(L, 2, 0);
                auto top = lua_gettop(L);
                lua_pushstring(L, "degree");
                lua_pushinteger(L, chordOption.degree);
                lua_settable(L, top);
                top = lua_gettop(L);
                lua_pushstring(L, "semitone");
                lua_pushinteger(L, chordOption.value);
                lua_settable(L, top);
            }
            void LuaChord::pushChordDegrees(lua_State *L)
            {
                auto top = lua_gettop(L);
                lua_pushstring(L, "degrees");
                lua_createtable(L, chordDef->intervals.size(), 0);
                int idx = 1;
                for (const auto &chordOption : chordDef->intervals) {
                    auto stop = lua_gettop(L);
                    lua_pushinteger(L, idx++);
                    pushChordDegree(L, chordOption);
                    lua_settable(L, stop);
                }
                lua_settable(L, top);
            }


            template<int Degree>
            bool hasDegreeImpl(const ChordDef &def) {
                return false;
            }
            template<>
            bool hasDegreeImpl<7>(const ChordDef &def) {
                return has7(def);
            }
            template<>
            bool hasDegreeImpl<9>(const ChordDef &def) {
                return has9(def);
            }
            template<>
            bool hasDegreeImpl<11>(const ChordDef &def) {
                return has11(def);
            }
            template<>
            bool hasDegreeImpl<13>(const ChordDef &def) {
                return has13(def);
            }
            template <int Degree>
            static int luaHas7 (lua_State *L) {
                using namespace sheet::lua;
                auto luaChord = ALuaObject::getObject<LuaChord>(L, -1);
                lua_pushboolean(L, hasDegreeImpl<Degree>(*(luaChord->chordDef)));
                return 1;
            }

            static const luaL_Reg libfs[] = {
                {"has7",   luaHas7<7>},
                {"has9",   luaHas7<9>},
                {"has11",   luaHas7<11>},
                {"has13",   luaHas7<13>},
                {NULL, NULL}
            };

            void LuaChord::push(lua_State *L)
            {
                Base::push(L, libfs, 1);
                pushChordName(L);
                pushChordDegrees(L);
            }
        }
        namespace luaPitches {
            struct LuaPitches : lua::ALuaObject {
                typedef lua::ALuaObject Base;
                const VoicingStrategy::Degrees *degrees;
                LuaPitches(const VoicingStrategy::Degrees *degrees)
                    : degrees(degrees)
                {}
                void push(lua_State *L);
                void pushDegrees(lua_State *L);
                void pushDegree(lua_State *L, const PitchDef &def);
            };

            void LuaPitches::pushDegree(lua_State *L, const PitchDef &def)
            {
                lua_createtable(L, 2, 0);
                auto top = lua_gettop(L);
                lua_pushstring(L, "degree");
                lua_pushinteger(L, def.pitch);
                lua_settable(L, top);
                top = lua_gettop(L);
                lua_pushstring(L, "octave");
                lua_pushinteger(L, def.octave);
                lua_settable(L, top);
            }

            void LuaPitches::pushDegrees(lua_State *L)
            {
                auto top = lua_gettop(L);
                lua_pushstring(L, "degrees");
                std::size_t idx = 1;
                lua_createtable(L, degrees->size(), 0);
                for(const auto &degree : *degrees) {
                    auto stop = lua_gettop(L);
                    lua_pushinteger(L, idx++);
                    pushDegree(L, degree);
                    lua_settable(L, stop);
                }
                lua_settable(L, top);
            }
            
            void LuaPitches::push(lua_State *L)
            {
                Base::push(L, NULL, 0);
                pushDegrees(L);
            }
        }        
    }
}

namespace sheet {
    namespace compiler {
        LuaVoicingStrategy::LuaVoicingStrategy(const fm::String &path) : LuaBase(path)
        {
        }

        bool LuaVoicingStrategy::canExecute() const 
        {
            return LuaBase::hasFunction(LUA_VOICING_STRATEGY_FENTRY);
        }

        LuaVoicingStrategy::Pitches LuaVoicingStrategy::get(const Event &chord, 
            const ChordDef &def, 
            const Degrees &degreeIntervals, 
            const TimeInfo&)
        {
            lua_getglobal(L, LUA_VOICING_STRATEGY_FENTRY);
            luaChord::LuaChord luaChord(&def, &chord);
            luaChord.push(L);
            luaPitches::LuaPitches luaPitches(&degreeIntervals);
            luaPitches.push(L);
            call(2, 0);
            return Pitches();
        }
    }
}