#include "Expression.h"
#include <compiler/commands/SetExpression.h>
#include <compiler/error.hpp>

namespace documentModel
{
    namespace conductor
    {
        bool Expression::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto& argument : arguments) {
                auto argumentExpression = compiler::SetExpression::getExpressionForString(argument.name, com::expression::Undefined);
                if (argumentExpression == com::expression::Undefined) 
                {
                    FM_THROW(compiler::Exception, "unknown expression symbol: " + argument.name);
                }
                if (argumentExpression == ev.contextInformation.expression) 
                {
                    return true;
                }
            }
            return false;
        }
    }
}
