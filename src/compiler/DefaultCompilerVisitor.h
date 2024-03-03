#pragma once

#include "ICompilerVisitor.h"

namespace compiler
{
    class DefaultCompilerVisitor : public ICompilerVisitor
    {
    public:
        virtual void beginCompile() override {}
        virtual void endCompile() override {}
        virtual void visitInstrument(const com::String &uname, const com::String instrumentSectionName) override {};
        virtual void visit(IContext *context, const documentModel::Event &ev) override {}
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override {}
        virtual void beginRenderPitch(const documentModel::PitchDef &pitch) override {}
        virtual void endRenderPitch() override {}
        virtual void beginDegreeRendering() override {}
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) override  {}
        virtual void endDegreeRendering() override {}
        virtual void beginRenderPhrase(const com::String& phraseName) override {}
        virtual void endRenderPhrase(const com::String& phraseName) override {}
        virtual void beginRenderTemplate(const com::String& templateName) override {}
        virtual void endRenderTemplate() override {}
        virtual void beginRenderVoice(int voiceNumber) override {}
        virtual void endRenderVoice() override {}
        virtual ~DefaultCompilerVisitor() = default;
    };
}
