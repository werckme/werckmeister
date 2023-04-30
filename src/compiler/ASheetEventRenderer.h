#pragma once

#include <functional>
#include <compiler/error.hpp>
#include <memory>
#include <compiler/context/IContext.h>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class ASheetEventRenderer
    {
    public:
        virtual void addEvent(const documentModel::Event &event) = 0;
        virtual void handleMetaEvent(const documentModel::Event &_ev) = 0;
        virtual std::shared_ptr<ASheetEventRenderer> createNewSheetEventRenderer(IContextPtr ctx) = 0;
        virtual void renderPhrase(const documentModel::Event &phraseEvent) = 0;
        virtual void renderDegree(const documentModel::Event &degreeEvent) = 0;
        template <class TContainer>
        void handleMetaEvents(const TContainer &container,
                              std::function<documentModel::Event(const typename TContainer::value_type &)> fGetMetaEvent);
    };
    typedef std::shared_ptr<ASheetEventRenderer> ASheetEventRendererPtr;
    //---------------------------------------------------------------------
    template <class TContainer>
    void ASheetEventRenderer::handleMetaEvents(const TContainer &container,
                                               std::function<documentModel::Event(const typename TContainer::value_type &)> fGetMetaEvent)
    {
        for (const auto &x : container)
        {
            const documentModel::Event &metaEvent = fGetMetaEvent(x);
            try
            {
                handleMetaEvent(metaEvent);
            }
            catch (const com::Exception &ex)
            {
                ex << ex_sheet_source_info(x);
                throw;
            }
        }
    }
}
