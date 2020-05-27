#ifndef COMPILER_SHEET_EVENT_RENDERER_H
#define COMPILER_SHEET_EVENT_RENDERER_H

#include <sheet/objects/Event.h>
#include "context/AContext.h"
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        class SheetEventRenderer {
        public:
            SheetEventRenderer(AContext *ctx) : ctx_(ctx) {}
            virtual ~SheetEventRenderer() = default;
            AContext* context() const { return this->ctx_; }
            virtual void addEvent(const Event &event);
            virtual void handleMetaEvent(const Event &_ev);
            virtual void __renderEvent__(const Event &_ev);
            virtual void __renderEventPitches__(const Event &noteEvent);
            virtual void __renderPitchBendEvent__(const Event &pitchBendEvent);

            template<class TContainer>
            void handleMetaEvents(const TContainer &container, 
                            std::function<sheet::Event(const typename TContainer::value_type&)> fGetMetaEvent);        
        protected:
            void onWarning(const fm::String &message, const Event &event);
        private:
            AContext* ctx_;
        };

        template<class TContainer>
        void SheetEventRenderer::handleMetaEvents(const TContainer &container, 
                        std::function<sheet::Event(const typename TContainer::value_type&)> fGetMetaEvent)
        {
            for(const auto &x : container) {
                Event metaEvent = fGetMetaEvent(x);
                try {
                    handleMetaEvent(metaEvent);
                } catch(fm::Exception &ex) {
                    ex << ex_sheet_source_info(x);
                    throw;
                }
            }
        }  
    }
}

#endif