#pragma once

#include "IPhraseRenderer.h"


namespace compiler
{
    class PhraseRenderer : public IPhraseRenderer
    {
    public:
        virtual void renderPhrase(const documentModel::Event &phraseEvent) override;
    };
    typedef std::shared_ptr<IPhraseRenderer> IPhraseRendererPtr;
}
