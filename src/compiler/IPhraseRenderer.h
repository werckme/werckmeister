#pragma once

#include <memory>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class IPhraseRenderer
    {
    public:
        virtual void renderPhrase(const documentModel::Event &phraseEvent) = 0;
    };
    typedef std::shared_ptr<IPhraseRenderer> IPhraseRendererPtr;
}
