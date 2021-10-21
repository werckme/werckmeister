#ifndef COMPILER_SHEET_EVENT_RENDERER_H
#define COMPILER_SHEET_EVENT_RENDERER_H

#include <sheet/objects/Event.h>
#include "context/IContext.h"
#include <compiler/error.hpp>
#include "ASheetEventRenderer.h"
#include <fm/ILogger.h>
#include "ICompilerVisitor.h"
#include <list>

namespace sheet {
    namespace compiler {
        class SheetEventRenderer : public ASheetEventRenderer {
        public:
            SheetEventRenderer(IContextPtr ctx, ICompilerVisitorPtr compilerVisitor, fm::ILoggerPtr logger) 
                : ctx_(ctx), compilerVisitor_(compilerVisitor), logger_(logger) 
            {}
            virtual ~SheetEventRenderer() = default;
            IContextPtr context() const { return this->ctx_; }
            virtual void addEvent(const Event &event);
            virtual void handleMetaEvent(const Event &_ev);
            virtual void renderEvent(const Event &_ev);
            virtual void renderEventPitches(const Event &noteEvent);
            virtual void renderPitchBendEvent(const Event &pitchBendEvent);
            virtual std::shared_ptr<ASheetEventRenderer> createNewSheetEventRenderer(IContextPtr ctx);
        protected:
            void onWarning(const fm::String &message, const Event &event);
        private:
            IContextPtr ctx_;
            fm::ILoggerPtr logger_;
            ICompilerVisitorPtr compilerVisitor_;
        };
    }
}
#endif