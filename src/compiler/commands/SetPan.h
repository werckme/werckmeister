#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="pan" where="voice">
    ///     set the pan of the current track
    /// ### example
    /// `/pan: 50/`
    /// </command>
    /// <param name="to" position="0" type="0..100">The pan value. Where 50 means the center</param>
    class SetPan : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetVolume.SetTo, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
