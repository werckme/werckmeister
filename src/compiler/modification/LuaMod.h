#pragma once

#include "AModification.h"
#include <lua/ALuaScript.h>
#include <com/common.hpp>
#include <forward.hpp>
#include <lua/ALuaWithParameter.h>

#define LUA_MODIFICATION_FENTRY "perform"

namespace compiler
{
    class LuaModification : public AModification,
                            public lua::ALuaScript,
                            public lua::ALuaWithParameter
    {
    public:
        typedef VoicingStrategy Base;
        typedef lua::ALuaScript LuaBase;
        LuaModification(const com::String &path);
        virtual ~LuaModification() = default;
        virtual bool canExecute() const override;
        virtual void assertCanExecute() const override;
        virtual void perform(IContextPtr ctx, Events &inOutEvents) override;
        virtual ParametersByNames &getParameters() override;

    protected:
        void pushEvents(IContextPtr ctx, const Events &events);
        Events popEvents(IContextPtr ctx);
        void popNoteEvent(documentModel::Event &event);
        void popPitchBendEvent(documentModel::Event &event);
        void popAndExecuteCc(IContextPtr ctx);
    private:
        documentModel::Event::Args args_;
    };
}
