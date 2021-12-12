#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    class NopCommand : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr) {}
    };
}
