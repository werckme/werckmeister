#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include "AUsingAnEvent.h"

namespace compiler
{
    class AddVorschlag : public ACommand, public AUsingAnEvent
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
