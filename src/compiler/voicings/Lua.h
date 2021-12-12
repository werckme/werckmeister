#pragma once

#include "VoicingStrategy.h"
#include <com/lua/ALuaScript.h>
#include <com/common.hpp>
#include <forward.hpp>
#include <com/lua/ALuaWithParameter.h>

#define LUA_VOICING_STRATEGY_FENTRY "solve"

namespace compiler
{
    class LuaVoicingStrategy : public VoicingStrategy,
                               public lua::ALuaScript,
                               public com::lua::ALuaWithParameter
    {
    public:
        typedef VoicingStrategy Base;
        typedef lua::ALuaScript LuaBase;
        LuaVoicingStrategy(const com::String &path);
        virtual ~LuaVoicingStrategy() = default;
        virtual bool canExecute() const override;
        virtual void assertCanExecute() const override;
        virtual Pitches get(const Event &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo &) override;
        virtual ParametersByNames &getParameters() override;

    protected:
    private:
        Pitches popPitches(lua_State *L);
        PitchDef popPitch(lua_State *L);
        Event::Args args_;
    };
}
