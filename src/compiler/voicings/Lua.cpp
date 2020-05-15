#include "Lua.h"
#include <lua.hpp>
#include <fm/lua/ALuaObject.h>
#include <fm/tools.h>
#include <compiler/error.hpp>
#include <algorithm>
#include <compiler/lua/luaTimeInfo.h>

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
                void pushChordDegree(lua_State *L, const DegreeDef &DegreeDef);
            };
            void LuaChord::pushChordName(lua_State *L)
            {
                auto chordElements = chordEvent->chordElements();
                auto base = std::get<0>(chordElements);
                auto options = std::get<1>(chordElements);
                auto strBase = pitchToString(base);
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
            void LuaChord::pushChordDegree(lua_State *L, const DegreeDef &DegreeDef)
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
                lua_createtable(L, chordDef->intervals.size(), 0);
                for (const auto &DegreeDef : chordDef->intervals) {
                    pushChordDegree(L, DegreeDef);
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
            const char * LuaPitchKeyPitch = "pitch";
            const char * LuaPitchKeyOctave = "octave";
            const char * LuaPitchKeyRoot = "root";
            const char * LuaPitchKeyDegreeValue = "degreeValue"; 
            enum { NoDegreeValue= INT_MAX };           
            struct LuaPitches : lua::ALuaObject {
                typedef lua::ALuaObject Base;
                const ChordDef *chordDef;
                const Event *chordEvent;
                const LuaVoicingStrategy::Degrees *degrees;
                LuaPitches(const ChordDef *chordDef, const Event *chordEvent, const VoicingStrategy::Degrees *degrees)
                    : chordDef(chordDef), chordEvent(chordEvent), degrees(degrees)
                {}
                void push(lua_State *L);
                void pushDegrees(lua_State *L);
                void pushDegrees(lua_State *L, PitchDef::Pitch root, int degreeValue, const std::vector<PitchDef> &degrees);
                void pushDegree(lua_State *L, PitchDef::Pitch root, int degreeValue,  PitchDef::Octave octave);
            };

            void LuaPitches::pushDegree(lua_State *L, PitchDef::Pitch root, int degreeValue, PitchDef::Octave octave)
            {
                lua_createtable(L, 2, 0);
                auto top = lua_gettop(L);
                lua_pushstring(L, LuaPitchKeyOctave);
                lua_pushinteger(L, octave);
                lua_settable(L, top);
                if (degreeValue == NoDegreeValue) {
                    return;
                }
                top = lua_gettop(L);
                lua_pushstring(L, LuaPitchKeyDegreeValue);
                lua_pushinteger(L, degreeValue);
                lua_settable(L, top);         
            }

            void LuaPitches::pushDegrees(lua_State *L)
            {
                if (degrees->empty()) {
                    return;
                }
                auto chordElements = chordEvent->chordElements();
		        auto root = std::get<0>(chordElements);
                std::vector<PitchDef> orderedByPitch(degrees->begin(), degrees->end());
                auto degreeComp = [](const PitchDef &a, const PitchDef &b){ return getDegreeValue(a.pitch) < getDegreeValue(b.pitch); };
                std::sort(orderedByPitch.begin(), orderedByPitch.end(), degreeComp);
                auto it = orderedByPitch.begin();
                while (it < orderedByPitch.end())  {
                    // <I I'> => I=[{I}, {I'}]
                    // II => II=[{II}] 
                    auto degreeValue = getDegreeValue(it->pitch);
                    auto grouped = std::equal_range(orderedByPitch.begin(), orderedByPitch.end(), *it, degreeComp); 
                    std::vector<PitchDef> degreesRanged(grouped.first, grouped.second);
                    pushDegrees(L, root, degreeValue, degreesRanged);
                    it = grouped.second; // it++
		        }
            }
            
            void LuaPitches::pushDegrees(lua_State *L, PitchDef::Pitch root, int degreeValue, const std::vector<PitchDef> &degrees_)
            {
                auto luaStackMainTable = lua_gettop(L);
                lua_pushinteger(L, degreeValue);
                lua_createtable(L, degrees_.size(), 0);
                auto luaStackDegrees = lua_gettop(L);
                int degreeIndex = 1;
                for (const auto &degree : degrees_) {
                    auto degreeDef = chordDef->getDegreeDef(degree);
                    if (!degreeDef.valid()) {
                        degreeDef.value = NoDegreeValue;
                    }
                    lua_pushinteger(L, degreeIndex++);
                    pushDegree(L, root, degreeDef.value, degree.octave);
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

        void LuaVoicingStrategy::assertCanExecute() const 
        {
            if (!canExecute()) {
                FM_THROW(Exception, fm::String("missing '") + LUA_VOICING_STRATEGY_FENTRY + "' function");
            }
        }
        
        PitchDef LuaVoicingStrategy::popPitch(lua_State *L)
        {
            PitchDef result;
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
            if (!lua_istable(L, -1)) {
                FM_THROW(Exception, "lua result is not a table");
            }
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (!lua_istable(L, -1)) {
                    lua_pop(L, 1);
                    continue;
                }
                result.insert(popPitch(L));
                lua_pop(L, 1);
            }
            return result;
        }

        LuaVoicingStrategy::Pitches LuaVoicingStrategy::get(const Event &chord, 
            const ChordDef &def, 
            const Degrees &degreeIntervals, 
            const TimeInfo& t)
        {
            lua_getglobal(L, LUA_VOICING_STRATEGY_FENTRY);
            luaChord::LuaChord luaChord(&def, &chord);
            luaChord.push(L);
            luaPitches::LuaPitches luaPitches(&def, &chord, &degreeIntervals);
            luaPitches.push(L);
            pushArgs(this->args_);
            lua::LuaTimeInfo(t).push(L);
            call(4, 1);
            return popPitches(L);
        }

        void LuaVoicingStrategy::pushArgs(const Event::Args &args)
        {
            // #74.2 TODO
            // lua_createtable(L, 0, 0);
            // if (args.size() == 1) { // first arg is the script name
            //     return;
            // }
            // auto top = lua_gettop(L);
            // auto it = args.begin() + 1;
            // for(; it < args.end(); ++it) {
            //     lua_pushinteger(L, it - args.begin());
            //     lua_pushstring(L, it->c_str());
            //     lua_settable(L, top);
            // }
        }

        void LuaVoicingStrategy::setArguments(const Event::Args &args)
        {
            this->args_ = args;
        }
    }
}