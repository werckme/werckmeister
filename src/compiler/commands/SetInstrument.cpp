#include "SetInstrument.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetInstrument::execute(AContext* context)
        {
            auto name         = parameters[argumentNames.SetInstrument.Use].value<fm::String>();
            context->setInstrument(name);           
        }
    }
}