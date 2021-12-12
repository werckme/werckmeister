#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="instrument" where="trackConfig, track">
    /// Set or change the instrument of a track.
    /// ### examples
    /// **set an instrument for a track**
    /// ```
    /// [ \n
    /// instrument: myInstrument; \n
    /// { \n
    ///   c d e f \n
    /// } \n
    /// ] \n
    /// ```
    /// **change current instrument within a track**
    /// ```
    /// [ \n
    /// { \n
    ///   c d e f | \n
    ///   /instrument: myOtherInstrument/
    ///   c d e f
    /// } \n
    /// ] \n
    /// ```
    /// </command>
    /// <param name="use" position="0" type="text">The instrument name.</param>
    class SetInstrument : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetInstrument.Use, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
