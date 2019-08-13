#ifndef COMPILER_SHEET_EVENT_RENDERER_H
#define COMPILER_SHEET_EVENT_RENDERER_H

#include "sheet/Event.h"
#include "AContext.h"

namespace sheet {
    namespace compiler {
        class SheetEventRenderer {
        public:
            SheetEventRenderer(AContextPtr ctx) : ctx_(ctx) {}
            virtual ~SheetEventRenderer() = default;
            void render(const Event &event);
            AContextPtr context() const { return this->ctx_; }
        private:
            AContextPtr ctx_;
        };
    }
}

#endif