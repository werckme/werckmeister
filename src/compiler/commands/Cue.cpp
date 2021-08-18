#include "Cue.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void Cue::execute(IContextPtr  context)
        {
            auto meta = context->voiceMetaData();
            auto text = parameters[argumentNames.Cue.Text].value<fm::String>();      
            context->addCue(text, meta->position);
        }
    }
}