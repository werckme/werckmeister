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
        /// <param name="from"     position="1" default="0" type="0..100">The source volume value.</param>
        /// <param name="to"       position="2" default="100" type="0..100">The target volume value.</param>
        /// <param name="curve"    position="3" default="lin" type="lin|quad|cub|quart|quint|exp">The fade curve type.</param>
        class SetFade : public ACommand
        {
        public:
            static const fm::String CurveTypeLin;
            static const fm::String CurveTypeQuad;
            static const fm::String CurveTypeCub;
            static const fm::String CurveTypeQuart;
            static const fm::String CurveTypeQuint;
            static const fm::String CurveTypeExp;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetFade.FadeDuration, 	0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.SetFade.FadeFrom,    	1, "0"),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.SetFade.FadeTo, 	        2, "100"),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.SetFade.Curve, 	        3, CurveTypeLin)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif