#pragma once

#include <documentModel/objects/Event.h>
#include "context/IContext.h"
#include <compiler/error.hpp>
#include "ASheetEventRenderer.h"
#include <com/ILogger.h>
#include "ICompilerVisitor.h"
#include <list>

namespace documentModel {
    namespace compiler {
        class SheetEventRenderer : public ASheetEventRenderer {
        public:
            SheetEventRenderer(IContextPtr ctx, ICompilerVisitorPtr compilerVisitor, com::ILoggerPtr logger) 
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
            void onWarning(const com::String &message, const Event &event);
        private:
            IContextPtr ctx_;
            com::ILoggerPtr logger_;
            ICompilerVisitorPtr compilerVisitor_;
        };
    }
}
