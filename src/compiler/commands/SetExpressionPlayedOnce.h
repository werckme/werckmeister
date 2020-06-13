#ifndef SETEXPRESSIONPLAYEDONCE_HPP
#define SETEXPRESSIONPLAYEDONCE_HPP

#include "SetExpression.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class SetExpressionPlayedOnce : public SetExpression
        {
        public:
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif