#ifndef SETEXPRESSIONPLAYEDONCE_HPP
#define SETEXPRESSIONPLAYEDONCE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class SetExpressionPlayedOnce : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                //FM_PARAMETER_DEF		    (argumentNames.XYZ, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif