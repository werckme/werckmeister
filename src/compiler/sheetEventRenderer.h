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
            void addEvent(const Event &event);
            AContext* context() const { return this->ctx_; }
            void renderEvent(const Event &_ev);
        protected:
        private:
            AContext* ctx_;
        };
    }
}

#endif