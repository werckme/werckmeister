#include "SetSignature.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetSignature::execute(AContext* context)
        {
            auto upper         = parameters[argumentNames.SetSignature.Upper].value<int>();
            auto lower         = parameters[argumentNames.SetSignature.Lower].value<int>();
            context->setSignature(upper, lower);
        }
    }
}