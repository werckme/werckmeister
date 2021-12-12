#include "InstrumentConfigVelocityRemap.h"
#include <compiler/context/IContext.h>
#include <compiler/error.hpp>
#include <compiler/commands/SetExpression.h>

namespace compiler
{
    const com::String InstrumentConfigVelocityRemap::VelocityNotSet = "NoRemapValueSet";

    void InstrumentConfigVelocityRemap::execute(IContextPtr context)
    {
        if (this->hasInstrument() == false)
        {
            return;
        }
        auto instrumentDef = getInstrument();
        for (const auto &parametersKeyValue : parameters)
        {
            const auto &parameter = parametersKeyValue.second;
            auto hasValue = !parameter.empty() && parameter.strValue() != VelocityNotSet;
            if (!hasValue)
            {
                continue;
            }
            const auto &expression = parameter.name();
            double newValue = parameter.value<double>();
            if (newValue < 0 || newValue > 100)
            {
                FM_THROW(Exception, "invalid value for: " + expression + " (0..100)");
            }
            auto exprValue = SetExpression::getExpressionForString(expression);
            instrumentDef->velocityOverride[exprValue] = newValue;
        }
    }
}