#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"

namespace lua
{
    class PerformerScript : public IPerformerScript, public ALuaScript
    {
    public:
        PerformerScript() = default;
        virtual ~PerformerScript() = default;
        void scriptPath(const com::String &scriptPath);
        virtual bool canExecute() const { return false; }
        virtual void assertCanExecute() const {}
    };
    typedef std::shared_ptr<PerformerScript> PerformerScriptPtr;
}