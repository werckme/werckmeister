#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="chord" where="voice">
    ///    Sets a chord context for the [prhase](#phrases) rendering.
    ///    The given chord name must be defined in a *.chords file.
    /// #### example
    /// ```language=Werckmeister,type=full
    /// using "/chords/default.chords";\n
    /// device: myDevice midi _usePort=0;\n
    /// instrumentDef: piano  myDevice _ch=0 _pc=0;\n
    /// tempo: 75;\n
    /// myPhrase= I III V VII III';\n
    /// [\n
    /// instrument: piano;\n
    /// {\n
    ///     /chord: "C-7"/\n
    ///     >"myPhrase";\n
    /// }\n
    /// ]\n
    /// ```
    /// </command>
    /// <param name="name" type="text">the chord name</param>
    class SetChord : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetChord.Name, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
