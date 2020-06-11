#include "SetPan.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetPan::execute(IContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();      
            context->setPan(setTo);
        }
    }
}