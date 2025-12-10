#pragma once

#include <documentModel/objects/Event.h>
#include "context/IContext.h"
#include <compiler/error.hpp>
#include "ASheetEventRenderer.h"
#include <com/ILogger.h>
#include "ICompilerVisitor.h"
#include <list>
#include "IDefinitionsServer.h"
#include <set>

namespace compiler
{
    class SheetEventRenderer : public ASheetEventRenderer
    {
    public:
        SheetEventRenderer(IContextPtr ctx, ICompilerVisitorPtr compilerVisitor, com::ILoggerPtr logger, IDefinitionsServerPtr definitionServer)
            : ctx_(ctx), compilerVisitor_(compilerVisitor), logger_(logger), definitionServer_(definitionServer)
        {
        }
        virtual ~SheetEventRenderer() = default;
        IContextPtr context() const { return this->ctx_; }
        virtual void addEvent(const documentModel::Event &event) override;
        virtual void handleMetaEvent(const documentModel::Event &_ev) override;
        virtual void renderPhrase(const documentModel::Event &phraseEvent) override;
        virtual void renderDegree(const documentModel::Event &degreeEvent) override;
        virtual void renderEvent(const documentModel::Event &_ev);
        virtual void renderEventPitches(const documentModel::Event &noteEvent);
        virtual void renderPitchBendEvent(const documentModel::Event &pitchBendEvent);
        virtual void renderControllerEvent(const documentModel::Event &controllerEvent);
        virtual std::shared_ptr<ASheetEventRenderer> createNewSheetEventRenderer(IContextPtr ctx);
    protected:
        void onWarning(const com::String &message, const documentModel::Event &event);

    private:
        typedef std::set<com::String> PhraseNames;
        IContextPtr ctx_;
        ICompilerVisitorPtr compilerVisitor_;
        com::ILoggerPtr logger_;
        IDefinitionsServerPtr definitionServer_;
        void renderPhraseImpl(const documentModel::Event &phraseEvent, PhraseNames &phraseNamesCallStack);
    };
}
