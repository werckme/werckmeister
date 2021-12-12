#pragma once

#include "VoicingStrategy.h"
#include <lua/ALuaScript.h>
#include <com/common.hpp>
#include <forward.hpp>
#include <lua/ALuaWithParameter.h>

#define LUA_VOICING_STRATEGY_FENTRY "solve"

namespace compiler
{
    class LuaVoicingStrategy : public VoicingStrategy,
                               public lua::ALuaScript,
                               public lua::ALuaWithParameter
    {
    public:
        typedef VoicingStrategy Base;
        typedef lua::ALuaScript LuaBase;
        LuaVoicingStrategy(const com::String &path);
        virtual ~LuaVoicingStrategy() = default;
        virtual bool canExecute() const override;
        virtual void assertCanExecute() const override;
        virtual Pitches get(const documentModel::Event &chord, const documentModel::ChordDef &def, const Degrees &degreeIntervals, const TimeInfo &) override;
        virtual ParametersByNames &getParameters() override;

    protected:
    private:
        Pitches popPitches(lua_State *L);
        documentModel::PitchDef popPitch(lua_State *L);
        documentModel::Event::Args args_;
    };
}
