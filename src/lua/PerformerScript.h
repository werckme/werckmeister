#pragma once

#include <memory>
#include <com/common.hpp>
#include "IPerformerScript.h"
#include "ALuaScript.h"
#include <functional>

namespace sol
{
    class state_view;
}

namespace lua
{
    class PerformerScript : public IPerformerScript, public ALuaScript
    {
    public:
        struct LuaMidi
        {
            float position = 0;
        };
        typedef ALuaScript Base;
        PerformerScript() = default;
        virtual ~PerformerScript();
        void scriptPath(const com::String &scriptPath);
        virtual bool canExecute() const { return false; }
        virtual void assertCanExecute() const {}
        virtual void onMidiEvent(const com::midi::Event*) override;
    private:
        void jumpToPosition(double quarters);
        void initLuaFunctions(sol::state_view&);
        void initLuaTypes(sol::state_view&);
        sol::state_view* luaPtr = nullptr;
        /////////////////////
        std::function<void(LuaMidi)> luaOnMidiEvent = nullptr;
    };
    typedef std::shared_ptr<PerformerScript> PerformerScriptPtr;
}