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
            void addEvent(const Event &event);
            AContextPtr context() const { return this->ctx_; }
            void renderEvent(const Event &_ev);
        protected:
        private:
            AContextPtr ctx_;
        };
    }
}

#endif