#ifndef SETTEMPO_HPP
#define SETTEMPO_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="tempo" where="document, track">
        /// `tempo` defines or changes the current tempo.
        /// ### examples
        /// **positional:** 
        /// `tempo: 120;`
        /// **named:**
        /// `tempo: _bpm=120;`
        /// This command can be used as document config:
        /// `tempo: 120;`
        /// or within a track
        /// `/tempo: 120/`
        /// It is also possible to set different tempo values for several tracks:
        /// [see here](/manual#tempo)
        /// </command>
        /// <param name="bpm" position="0">The tempo bpm value.</param>
        class SetTempo : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetTempo.Bpm, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif