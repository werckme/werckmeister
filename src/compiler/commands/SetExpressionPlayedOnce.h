#pragma once

#include "SetExpression.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    class SetExpressionPlayedOnce : public SetExpression
    {
    public:
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
