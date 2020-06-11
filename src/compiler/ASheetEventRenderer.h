#ifndef ACOMPILER_SHEET_EVENT_RENDERER_H
#define ACOMPILER_SHEET_EVENT_RENDERER_H

#include <functional>
#include <compiler/error.hpp>

namespace sheet {
    struct Event;
    namespace compiler {
        class AContext;
        class ASheetEventRenderer {
        public:
            virtual void addEvent(const Event &event) = 0;
            virtual void handleMetaEvent(const Event &_ev) = 0;

            template<class TContainer>
            void handleMetaEvents(const TContainer &container, 
                            std::function<sheet::Event(const typename TContainer::value_type&)> fGetMetaEvent);        
        };
        //---------------------------------------------------------------------
        template<class TContainer>
        void ASheetEventRenderer::handleMetaEvents(const TContainer &container, 
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