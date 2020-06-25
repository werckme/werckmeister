#ifndef SETSIGNATURE_HPP
#define SETSIGNATURE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="signature">
        /// Set the signature of the current track.
        /// ### examples
        /// **positional:** 
        /// `/signature: 3 4/`
        /// **named:**
        /// `/signature: _upper=3 _lower=4/`
        /// </command>
        /// <param name="upper" position="0">The upper value of the signature.</param>
        /// <param name="lower" position="1">The lower value of the signature.</param>
        class SetSignature : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetSignature.Upper, 	0),
                FM_PARAMETER_DEF		    (argumentNames.SetSignature.Lower, 	1)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif