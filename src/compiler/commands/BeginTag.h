#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace compiler
{
    /// <command name="beginTag" where="voice">
    /// </command>
    /// <param name="tags" position="0" type="text">a space separated list of tag names</param>
    class BeginTag : public ACommand
    {
    public:
        typedef ACommand Base;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.BeginTag.Tags, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    protected:
    };
}
