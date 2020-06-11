#include "SetVolume.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetVolume::execute(IContext* context)
        {
            auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
            context->setVolume(setTo);
        }

    }
}