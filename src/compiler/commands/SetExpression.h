#ifndef SETEXPRESSION_HPP
#define SETEXPRESSION_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class SetExpression : public ACommand
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