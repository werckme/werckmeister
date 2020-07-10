#ifndef SHEET_SETPAN_HPP
#define SHEET_SETPAN_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="pan" where="track">
        ///     set the pan of the current track
        /// ### examples
        /// **positional:** 
        /// `/pan: 50/`
        /// **named:**
        /// `/pan: _to=50/`   
        /// </command>
        /// <param name="to" position="0" range="0..100">The pan value. Where 50 means the center</param>   
        class SetPan : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVolume.SetTo, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif