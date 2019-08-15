#ifndef MODIFICATION_LUA_SCRIPT_HPP
#define MODIFICATION_LUA_SCRIPT_HPP

#include "AModification.h"
#include <fm/lua/ALuaScript.h>
#include <fm/common.hpp>
#include <forward.hpp>

#define LUA_MODIFICATION_FENTRY "perform"

namespace sheet {
    namespace compiler {
        class LuaModification : public AModification, public lua::ALuaScript {
        public:
            typedef VoicingStrategy Base;
            typedef lua::ALuaScript LuaBase;
            LuaModification(const fm::String &path);
            virtual ~LuaModification() = default;
            virtual bool canExecute() const override;
            virtual void setArguments(const Event::Args &args) override;
            virtual void perform(AContext *ctx, const Event &ev) override;
        protected:
            void pushArgs(const Event::Args &args);
        private:
            Event::Args args_;
        };
    }
}

#endif