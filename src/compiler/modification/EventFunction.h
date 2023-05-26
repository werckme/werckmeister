#pragma once
#include "LuaMod.h"

#define LUA_EVENTFUNCTION_FENTRY "execute"

namespace compiler
{
    class LuaEventFunction : public LuaModification
    {
    public:
        typedef LuaModification Base;
        LuaEventFunction(const com::String &path);
        virtual bool canExecute() const override;
        virtual void assertCanExecute() const override;
        virtual void execute(IContextPtr ctx, Events &outEvents);
    protected:
    private:
    };
    typedef std::shared_ptr<LuaEventFunction> LuaEventFunctionPtr;
}
