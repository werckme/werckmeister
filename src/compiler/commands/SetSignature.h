#ifndef SETSIGNATURE_HPP
#define SETSIGNATURE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
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