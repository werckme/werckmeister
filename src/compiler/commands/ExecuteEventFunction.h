#pragma once

#include "AddMod.h"
#include <compiler/ASheetEventRenderer.h>
#include "AUsingAnEvent.h"

namespace compiler
{
    /// <command name="call" where="voice">
    /// Executes a lua function.
    /// See [Example](https://www.werckme.org/editor?wid=lua-event-functions-example)
    /// </command>
    /// <param name="name" position="0" type="text">The name of the lua function. This is the only "unique" parameter for this command. All further parameters are specific to its related implementation (see [The Execute Function](/manual#the-execute-function)).</param>
    class ExecuteEventFunction : public AddMod, public AUsingAnEvent
    {
    public:
        ExecuteEventFunction(ASheetEventRendererPtr sheetEventRenderer) : sheetEventRenderer_(sheetEventRenderer) {}
        typedef AddMod Base;
        virtual void execute(IContextPtr) override;
    protected:
        LuaEventFunctionPtr loadEventFunction(IContextPtr ctx, const com::String& eventFunctionName);
    private:
        ASheetEventRendererPtr sheetEventRenderer_;
    };
}
