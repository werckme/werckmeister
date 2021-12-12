#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="signature">
    /// Set the time signature of the current track.
    /// ### example
    /// `/signature: 3 4/`
    /// </command>
    /// <param name="upper" position="0">The upper value of the signature.</param>
    /// <param name="lower" position="1">The lower value of the signature.</param>
    class SetSignature : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetSignature.Upper, 0),
            FM_PARAMETER_DEF(argumentNames.SetSignature.Lower, 1)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
