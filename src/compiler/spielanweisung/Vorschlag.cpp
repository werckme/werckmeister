#include "Vorschlag.h"
#include <fm/literals.hpp>
#include <compiler/AContext.h>

namespace sheet {
    namespace compiler {
        using namespace fm;
        const fm::Ticks Vorschlag::defaultDuration = 1.0_N64;
        void Vorschlag::perform(AContext *ctx, const Event &ev)
        {
            auto meta = ctx->voiceMetaData();
            auto vorschlagDuration = vorschlagNote.duration != Event::NoDuration ? vorschlagNote.duration : defaultDuration;
            ctx->seek(-vorschlagDuration);
            auto preservedLastEventDuration = meta->lastEventDuration;
            Base::perform(ctx, vorschlagNote);
            meta->lastEventDuration = preservedLastEventDuration;
            Base::perform(ctx, ev);
        }
    }
}