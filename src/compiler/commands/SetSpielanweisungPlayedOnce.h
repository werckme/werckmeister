#pragma once

#include "SetSpielanweisung.h"

namespace compiler
{
    /// <command name="doOnce" where="voice">
    /// Like [do](#do). But with the difference, that the loaded mod will be only executed once for the following event.
    /// #### example
    /// `/doOnce: arpeggio/`
    /// </command>
    /// <param name="use" position="0" type="text">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>

    class SetSpielanweisungPlayedOnce : public SetSpielanweisung
    {
    public:
        virtual void execute(IContextPtr);
    };
}
