#ifndef COMPILER_SHEET_EVENT_RENDERER_H
#define COMPILER_SHEET_EVENT_RENDERER_H

#include <sheet/objects/Event.h>
#include "context/IContext.h"
#include <compiler/error.hpp>
#include "ASheetEventRenderer.h"

namespace sheet {
    namespace compiler {
        class SheetEventRenderer : public ASheetEventRenderer {
        public:
            SheetEventRenderer(IContextPtr ctx) : ctx_(ctx) {

            }
            virtual ~SheetEventRenderer() = default;
            IContextPtr context() const { return this->ctx_; }
            virtual void addEvent(const Event &event);
            virtual void handleMetaEvent(const Event &_ev);
            virtual void renderEvent(const Event &_ev);
            virtual void renderEventPitches(const Event &noteEvent);
            virtual void renderPitchBendEvent(const Event &pitchBendEvent);     
        protected:
            void onWarning(const fm::String &message, const Event &event);
        private:
            IContextPtr ctx_;
        };
    }
}
#endif