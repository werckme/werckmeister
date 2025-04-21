#include "SetChord.h"
#include <compiler/context/IContext.h>
#include <documentModel/objects/Event.h>
#include <compiler/metaCommands.h>

namespace compiler
{
    void SetChord::execute(IContextPtr context)
    {
        auto chordName = parameters[argumentNames.SetChord.Name].value<com::String>();
        documentModel::Event chordEvent;
        chordEvent.type = documentModel::Event::Chord;
        chordEvent.stringValue = chordName;
        context->currentChordEvent(chordEvent);
    }
}