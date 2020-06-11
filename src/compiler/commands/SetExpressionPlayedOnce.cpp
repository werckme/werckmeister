#include "SetExpressionPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetExpressionPlayedOnce::execute(IContext* context)
        {
            auto value         = parameters[argumentNames.SetExpression.Value].value<fm::String>();
            context->setExpressionPlayedOnce(getExpressionForString(value));  
        }
    }
}