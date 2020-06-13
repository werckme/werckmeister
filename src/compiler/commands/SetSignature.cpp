#include "SetSignature.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetSignature::execute(IContextPtr  context)
        {
            auto upper         = parameters[argumentNames.SetSignature.Upper].value<int>();
            auto lower         = parameters[argumentNames.SetSignature.Lower].value<int>();
            context->setSignature(upper, lower);
        }
    }
}