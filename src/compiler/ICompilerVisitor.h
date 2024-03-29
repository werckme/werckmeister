#pragma once

namespace com
{
    namespace midi
    {
        struct Event;
    }
}

#include <compiler/context/IContext.h>

namespace documentModel
{
    struct Event;
    struct PitchDef;
}

namespace compiler
{
    class ICompilerVisitor
    {
    public:
        virtual void beginCompile() = 0;
        virtual void endCompile() = 0;
        virtual void visitInstrument(const com::String &uname, const com::String instrumentSectionName = com::String()) = 0;
        virtual void visit(IContext *context, const documentModel::Event &ev) = 0;
        virtual void beginRenderPitch(const documentModel::PitchDef &pitch) = 0;
        virtual void endRenderPitch() = 0;
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) = 0;
        virtual void beginDegreeRendering() = 0;
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) = 0;
        virtual void endDegreeRendering() = 0;
        virtual void beginRenderPhrase(const com::String& phraseName) = 0;
        virtual void endRenderPhrase(const com::String& phraseName) = 0;
        virtual void beginRenderTemplate(const com::String& templateName) = 0;
        virtual void endRenderTemplate() = 0;
        virtual void beginRenderVoice(int voiceNumber) = 0;
        virtual void endRenderVoice() = 0;
    };
    typedef std::shared_ptr<ICompilerVisitor> ICompilerVisitorPtr;
}
