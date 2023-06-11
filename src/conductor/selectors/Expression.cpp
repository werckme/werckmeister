#include "Expression.h"
#include <compiler/commands/SetExpression.h>
#include <compiler/error.hpp>

namespace conductor
{
    bool Expression::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.midiEvent;
        for (const auto &argument : arguments)
        {
            auto argumentExpression = compiler::SetExpression::getExpressionForString(argument.name, com::expression::Undefined);
            if (argumentExpression == com::expression::Undefined)
            {
                FM_THROW(compiler::Exception, "unknown expression symbol: " + argument.name);
            }
            const auto* eventInfo = _eventInformationServer->find(*evm.midiEvent);
            if (!eventInfo) 
            {
                return false;
            }
            if (argumentExpression == eventInfo->expression)
            {
                return true;
            }
        }
        return false;
    }
}
