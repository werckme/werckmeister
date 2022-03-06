#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="cue" where="voice">
    ///     adds a cue MIDI meta message to the corresponding midi track
    /// ### examples
    /// `/cue: cueTtext/`
    /// &gt; If a cue was visited several times during a repeat or jump an additional repeat index will be added.\n
    /// &gt; For example:\n
    /// &gt; `/cue: myCue/ c d e f :|` will create two cue MIDI messages: "myCue" and "myCue1"
    /// &gt;
    /// </command>
    /// <param name="text" type="text">the text which appears in the MIDI message</param>
    class Cue : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.Cue.Text, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
