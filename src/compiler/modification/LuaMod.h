#ifndef MODIFICATION_LUA_SCRIPT_HPP
#define MODIFICATION_LUA_SCRIPT_HPP

#include "AModification.h"
#include <fm/lua/ALuaScript.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include <fm/lua/ALuaWithParameter.h>

#define LUA_MODIFICATION_FENTRY "perform"

namespace sheet {
    namespace compiler {
        class LuaModification : 
            public AModification, 
            public lua::ALuaScript,
            public fm::lua::ALuaWithParameter
        {
        public:
            typedef VoicingStrategy Base;
            typedef lua::ALuaScript LuaBase;
            LuaModification(const fm::String &path);
            virtual ~LuaModification() = default;
            virtual bool canExecute() const override;
            virtual void assertCanExecute() const override;
            virtual void perform(IContextPtr ctx, Events &events) override;
            virtual ParametersByNames & getParameters() override;
        protected:
            void pushEvents(IContextPtr ctx, const Events &events);
            Events popEvents();
            void popNoteEvent(Event &event);
            void popPitchBendEvent(Event &event);
        private:
            Event::Args args_;
        };
    }
}

#endif