#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="volume" where="voice">
    ///     set the volume of the current instrument
    /// ### examples
    /// `/volume: 50/`
    /// </command>
    /// <param name="to" position="0" type="0..100">The volume value.</param>
    class SetVolume : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetVolume.SetTo, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
