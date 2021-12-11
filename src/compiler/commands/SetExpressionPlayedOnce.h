#pragma once

#include "SetExpression.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        class SetExpressionPlayedOnce : public SetExpression
        {
        public:
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

