#ifndef COMPILER_SHEET_EVENT_RENDERER_H
#define COMPILER_SHEET_EVENT_RENDERER_H

#include "sheet/Event.h"
#include "AContext.h"

namespace sheet {
    namespace compiler {
        class SheetEventRenderer {
        public:
            SheetEventRenderer(AContext *ctx) : ctx_(ctx) {}
            virtual ~SheetEventRenderer() = default;
            AContext* context() const { return this->ctx_; }
            virtual void addEvent(const Event &event);
            virtual void __renderEvent__(const Event &_ev);
            virtual void __renderEventPitches__(const Event &noteEvent);
            virtual void __renderPitchBendEvent__(const Event &pitchBendEvent);
        protected:
        private:
            AContext* ctx_;
        };
    }
}

#endif