#include "Vorschlag.h"
#include <com/literals.hpp>
#include <compiler/context/AContext.h>

namespace documentModel {
    namespace compiler {
        using namespace com;
        const com::Ticks Vorschlag::defaultDuration = 1.0_N64;
        void Vorschlag::perform(IContextPtr ctx, Events &events)
        {
            if (events.empty()) {
                return;
            }
            Event &mainNote = events.front();
            auto vorschlagCopy = vorschlagNote;
            vorschlagCopy.type = Event::Note;
            vorschlagCopy.duration = vorschlagNote.duration != Event::NoDuration ? vorschlagNote.duration : defaultDuration;
            vorschlagCopy.velocity = ctx->velocity();
            events.push_front(vorschlagCopy);
            mainNote.duration -= vorschlagCopy.duration;
            mainNote.offset = vorschlagCopy.duration;
        }
    }
}