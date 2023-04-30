#pragma once

#include "IPhraseRenderer.h"
#include "IDefinitionsServer.h"
#include <com/ILogger.h>
#include "ASheetEventRenderer.h"
#include "ISheetTemplateRenderer.h"

namespace compiler
{
    class PhraseRenderer : public IPhraseRenderer
    {
    public:
        PhraseRenderer(com::ILoggerPtr logger, 
        IDefinitionsServerPtr definitionServer,
        ASheetEventRendererPtr eventRenderer,
        ISheetTemplateRendererPtr templateRenderer) : 
        logger_(logger)
        , definitionServer_(definitionServer)
        , eventRenderer_(eventRenderer)
        , templateRenderer_(templateRenderer)
        {
        }
        virtual void renderPhrase(const documentModel::Event &phraseEvent) override;
    private:
        com::ILoggerPtr logger_;
        IDefinitionsServerPtr definitionServer_;
        ASheetEventRendererPtr eventRenderer_;
        ISheetTemplateRendererPtr templateRenderer_;
        void renderImpl(const documentModel::Event &phraseEvent);
    };
    typedef std::shared_ptr<IPhraseRenderer> IPhraseRendererPtr;
}
