#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <map>
#include <com/units.hpp>

namespace compiler
{
    class SetExpression : public ACommand
    {
    public:
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetExpression.Value, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
        com::Expression static getExpressionForString(const com::String &expressionStr, com::Expression returnIfNotFound = com::expression::Default);

    private:
        typedef std::map<com::String, com::Expression> ExpressionMap;
        static const ExpressionMap _ExpressionMap;
    };
}
