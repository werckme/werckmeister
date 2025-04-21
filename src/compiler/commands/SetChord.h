#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="setChord" where="voice">
    ///    Sets a chord context to the current voice to be able to use relative [degrees](#degree-events) instead of absolute notes.
    ///  The given chord name must be defined in a `*.chords` file.
    ///  A special chord name is "null", this unsets the current chord, wich is usefull when used within a template, to switch bewteen `accomp` track chord and a given chord:
    ///  [https://www.werckme.org/editor?wid=release-1-4-5-chordname](https://www.werckme.org/editor?wid=release-1-4-5-chordname)
    /// #### example
    /// ```language=Werckmeister,type=full
    /// using "/chords/default.chords";\n
    /// device: myDevice midi _usePort=0;\n
    /// instrumentDef: piano  myDevice _ch=0 _pc=0;\n
    /// tempo: 120;\n
    /// [\n
    /// instrument: piano;\n
    /// {\n
    ///     /chord: "C-9"/\n
    ///     I III V VII | II'1 \n
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
