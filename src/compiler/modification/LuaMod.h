#pragma once

#include "AModification.h"
#include <com/lua/ALuaScript.h>
#include <com/common.hpp>
#include <forward.hpp>
#include <com/lua/ALuaWithParameter.h>

#define LUA_MODIFICATION_FENTRY "perform"

namespace compiler
{
    class LuaModification : public AModification,
                            public lua::ALuaScript,
                            public com::lua::ALuaWithParameter
    {
    public:
        typedef VoicingStrategy Base;
        typedef lua::ALuaScript LuaBase;
        LuaModification(const com::String &path);
        virtual ~LuaModification() = default;
        virtual bool canExecute() const override;
        virtual void assertCanExecute() const override;
        virtual void perform(IContextPtr ctx, Events &events) override;
        virtual ParametersByNames &getParameters() override;

    protected:
        void pushEvents(IContextPtr ctx, const Events &events);
        Events popEvents();
        void popNoteEvent(Event &event);
        void popPitchBendEvent(Event &event);

    private:
        Event::Args args_;
    };
}
