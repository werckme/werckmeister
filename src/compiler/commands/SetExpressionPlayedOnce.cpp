#include "SetExpressionPlayedOnce.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetExpressionPlayedOnce::execute(AContext* context)
        {
            auto value         = parameters[argumentNames.SetExpression.Value].value<fm::String>();
            context->setExpressionPlayedOnce(getExpression(value));  
        }
    }
}