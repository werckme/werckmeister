#include "SetInstrument.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetInstrument::execute(IContextPtr  context)
        {
            auto name         = parameters[argumentNames.SetInstrument.Use].value<fm::String>();
            context->setInstrument(name);           
        }
    }
}