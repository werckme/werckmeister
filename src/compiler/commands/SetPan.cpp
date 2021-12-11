#include "SetPan.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void SetPan::execute(IContextPtr  context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();      
            context->setPan(setTo);
        }
    }
}