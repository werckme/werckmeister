#include "InstrumentConfigVelocityRemap.h"
#include <compiler/context/AContext.h>
#include <compiler/error.hpp>
#include <compiler/commands/SetExpression.h>

namespace sheet {
    namespace compiler {
        const fm::String InstrumentConfigVelocityRemap::VelocityNotSet = "NoRemapValueSet";

        void InstrumentConfigVelocityRemap::execute(AContext* context)
        {
            if (this->hasInstrument() == false) {
                return;
            }
            auto instrumentDef = getInstrument();
            for (const auto& parametersKeyValue : parameters) {
                const auto& parameter = parametersKeyValue.second;
                auto hasValue = !parameter.empty() && parameter.strValue() != VelocityNotSet;
                if (!hasValue) {
                    continue;
                }
                const auto& expression = parameter.name();
                int newValue = parameter.value<int>();
                if (newValue < 0 || newValue > 100) {
                    FM_THROW(Exception, "invalid value for: " + expression);
                }
                auto exprValue = SetExpression::getExpressionForString(expression);
                instrumentDef->velocityOverride[exprValue] = newValue;
            }
        }
    }
}