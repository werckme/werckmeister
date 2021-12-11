#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        /// <command name="fade" where="track">
        ///     Fades the volume over a given duration in quarters.
        /// ### example
        /// ```language=Werckmeister,type=full
        /// device: myDevice midi _usePort=0;\n
        /// instrumentDef: organ  myDevice _ch=0 _pc=16;\n
        /// tempo: 75;\n
        /// [\n
        /// instrument: organ;\n
        /// {\n
        ///     \fff\n
        ///     /fade: _from=0 _to=100 _duration=4/\n
        ///     c1 |\n
        ///     /fade: _from=0 _to=100 _duration=4 _curve="quad"/\n
        ///     c1 |\n
        ///     /fade: _from=0 _to=100 _duration=4 _curve="cub"/\n
        ///     c1 |\n
        ///     /fade: _from=0 _to=100 _duration=4 _curve="quart"/\n
        ///     c1 |\n
        ///     /fade: _from=0 _to=100 _duration=4 _curve="quint"/\n
        ///     c1 |\n
        ///     /fade: _from=0 _to=100 _duration=4 _curve="exp"/\n
        ///     c1 |\n
        /// }\n
        /// ]\n
        /// ```
        /// ![supported curve types](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/curve-types.png)
        /// </command>
        /// <param name="duration" position="0" type="0..N">the duration in quarters.</param>
        /// <param name="from"     position="1" default="0" type="0..100">The source volume value.</param>
        /// <param name="to"       position="2" default="100" type="0..100">The target volume value.</param>
        /// <param name="curve"    position="3" default="lin" type="lin,quad,cub,quart,quint,exp">The fade curve type.</param>
        class SetFade : public ACommand
        {
        public:
            static const com::String CurveTypeLin;
            static const com::String CurveTypeQuad;
            static const com::String CurveTypeCub;
            static const com::String CurveTypeQuart;
            static const com::String CurveTypeQuint;
            static const com::String CurveTypeExp;
            com::IHasParameter::ParametersByNames parameters = {
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

