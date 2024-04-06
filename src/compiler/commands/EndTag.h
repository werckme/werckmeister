#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace compiler
{
    /// <command name="endTag" where="voice">
    ///     Ends a tag region where its including events will receive the given tags. See [beginTag](manual/#beginTag).
    ///     Example: TBD
    /// </command>
    /// <param name="tags" position="0" type="text">a space separated list of tag names</param>
    class EndTag : public ACommand
    {
    public:
        typedef ACommand Base;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.EndTag.Tags, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    protected:
    };
}
