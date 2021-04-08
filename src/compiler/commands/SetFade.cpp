#include "SetFade.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetFade::execute(IContextPtr  context)
        {
            throw std::runtime_error("?");
            // auto setTo = parameters[argumentNames.SetVolume.SetTo].value<double>();
            // context->setVolume(setTo);
        }

    }
}