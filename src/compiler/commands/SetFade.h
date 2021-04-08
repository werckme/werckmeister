#ifndef SHEET_SETFADE_HPP
#define SHEET_SETFADE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="fade" where="track">
        ///     fades the volume over a given duration in quarters
        /// ### examples
        /// **positional:** 
        /// `/fade: 2 0 50/` --fades from 0 to 50 in 2 quarters
        /// **named:**
        /// `/volume: _duration=2 _from=0 _to=50/` --fades from 0 to 50 in 2 quarters
        /// </command>
        /// <param name="duration" position="0" type="0..N">the duration in quarters.</param>
        /// <param name="from"     position="1" type="0..100">The source volume value.</param>
        /// <param name="to"       position="2" type="0..100">The target volume value.</param>
        class SetFade : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetFade.FadeDuration, 	0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.SetFade.FadeFrom,    	1, "0"),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.SetFade.FadeTo, 	        2, "100")
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif