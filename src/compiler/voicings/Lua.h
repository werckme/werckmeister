#ifndef VOICINGS_LUA_SCRIPT_HPP
#define VOICINGS_LUA_SCRIPT_HPP

#include "VoicingStrategy.h"
#include <fm/lua/ALuaScript.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include <fm/lua/ALuaWithParameter.h>

#define LUA_VOICING_STRATEGY_FENTRY "solve"

namespace sheet {
    namespace compiler {
        class LuaVoicingStrategy : public VoicingStrategy, 
            public lua::ALuaScript, 
            public fm::lua::ALuaWithParameter
    {
        public:
            typedef VoicingStrategy Base;
            typedef lua::ALuaScript LuaBase;
            LuaVoicingStrategy(const fm::String &path);
            virtual ~LuaVoicingStrategy() = default;
            virtual bool canExecute() const override;
            virtual void assertCanExecute() const override;
            virtual Pitches get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) override;
            virtual ParametersByNames & getParameters() override;
        protected:
        private:
            Pitches popPitches(lua_State *L);
            PitchDef popPitch(lua_State *L);
            Event::Args args_;
        };
    }
}

#endif