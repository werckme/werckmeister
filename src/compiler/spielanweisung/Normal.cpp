#include "Normal.h"
#include "compiler/AContext.h"

namespace sheet {
    namespace compiler {

        void Normal::perform(AContext *ctx, const Event &ev)
        {
            const auto& pitches = ev.pitches;
			auto duration = ev.duration;
			auto tying = ev.isTying(); 
            auto meta = ctx->voiceMetaData();
			for (const auto &pitch : pitches)
			{
				ctx->renderPitch(pitch, duration, ev.velocity, tying);
			}
			ctx->seek(duration);
        }

    }
}