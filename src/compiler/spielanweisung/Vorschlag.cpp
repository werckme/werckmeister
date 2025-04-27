#include "Vorschlag.h"
#include <com/literals.hpp>
#include <compiler/context/AContext.h>
#include <compiler/IDefinitionsServer.h>

namespace compiler
{
    using namespace com;
    const com::Ticks Vorschlag::defaultDuration = 1.0_N64;


    Vorschlag::Vorschlag(IDefinitionsServerPtr definitionsServer) 
    : definitionsServer(definitionsServer)
    {

    }

    void Vorschlag::perform(IContextPtr ctx, Events &events)
    {
        using namespace documentModel;
        if (events.empty())
        {
            return;
        }
        Event &mainNote = events.front();
        auto vorschlagCopy = vorschlagNote;
        vorschlagCopy.type = vorschlagNote.subType;
        vorschlagCopy.duration = vorschlagNote.duration != Event::NoDuration ? vorschlagNote.duration : defaultDuration;
        vorschlagCopy.velocity = ctx->velocity();
        if (vorschlagCopy.type == Event::Degree) 
        {
            const auto &chord = ctx->currentChordEvent();
            definitionsServer->degreeToAbsoluteNote(ctx, chord, vorschlagCopy, vorschlagCopy);
        }
        events.push_front(vorschlagCopy);
        mainNote.duration -= vorschlagCopy.duration;
        mainNote.offset = vorschlagCopy.duration;
    }
}