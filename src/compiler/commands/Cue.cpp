#include "Cue.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void Cue::execute(IContextPtr context)
    {
        auto meta = context->voiceMetaData();
        auto text = parameters[argumentNames.Cue.Text].value<com::String>();
        context->addCue(text, meta->position);
    }
}