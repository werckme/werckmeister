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
        virtual void visit(const documentModel::PitchDef &pitch) override {}
        virtual void beginDegreeRendering() override {}
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) override  {}
        virtual void endDegreeRendering() override {}
        virtual ~DefaultCompilerVisitor() = default;
    };
}
