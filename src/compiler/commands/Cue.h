#ifndef SHEET_CUE_HPP
#define SHEET_CUE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="cue" where="track">
        ///     adds a cue meta message to the corresponding midi track
        /// ### examples
        /// `/cue: _text="cue text"/`   
        /// </command>
        /// <param name="text" type="text">the text which appears in the MIDI message</param>   
        class Cue : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.Cue.Text, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif