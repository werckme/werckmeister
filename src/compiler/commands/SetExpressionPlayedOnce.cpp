#include "SetExpressionPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void SetExpressionPlayedOnce::execute(IContextPtr  context)
        {
            auto value         = parameters[argumentNames.SetExpression.Value].value<com::String>();
            context->setExpressionPlayedOnce(getExpressionForString(value));  
        }
    }
}