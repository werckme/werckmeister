#pragma once

#include "ICompilerVisitor.h"

namespace compiler
{
    class DefaultCompilerVisitor : public ICompilerVisitor
    {
    public:
        virtual void beginCompile() override {}
        virtual void endCompile() override {}
        virtual void visitInstrument(const com::String &, const com::String) override {};
        virtual void visit(IContext *, const documentModel::Event &) override {}
        virtual void visit(IContext *, const com::midi::Event &, IContext::TrackId) override {}
        virtual void beginRenderPitch(const documentModel::PitchDef &) override {}
        virtual void endRenderPitch() override {}
        virtual void beginDegreeRendering() override {}
        virtual void visitDegree(const documentModel::Event&, const documentModel::ChordDef&, const documentModel::Event&) override  {}
        virtual void endDegreeRendering() override {}
        virtual void beginRenderPhrase(const com::String&) override {}
        virtual void endRenderPhrase(const com::String&) override {}
        virtual void beginRenderTemplate(const com::String&) override {}
        virtual void endRenderTemplate() override {}
        virtual void beginRenderVoice(int) override {}
        virtual void endRenderVoice() override {}
        virtual ~DefaultCompilerVisitor() = default;
    };
}
