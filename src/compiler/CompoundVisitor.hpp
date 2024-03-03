#pragma once

#include <compiler/ICompilerVisitor.h>

namespace compiler
{
    template<class TVisitor1>
    class CompoundVisitor_N1 : public ICompilerVisitor
    {
    public:
        typedef CompoundVisitor_N1<TVisitor1> TClass;
        typedef std::shared_ptr<TVisitor1> TVisitor1Ptr;
        typedef std::shared_ptr<TClass> TSharedPtr;
        CompoundVisitor_N1(TVisitor1Ptr visitor1) 
            : visitor1(visitor1)
        {
        }
        virtual ~CompoundVisitor_N1() = default;
        virtual void beginCompile() override 
        {
            visitor1->beginCompile();
        }
        virtual void endCompile() override 
        {
            visitor1->endCompile();
        }
        virtual void visit(IContext *context, const documentModel::Event &ev) override 
        {
            visitor1->visit(context, ev);
        }
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override 
        {
            visitor1->visit(context, ev, trackId);
        }
        virtual void beginDegreeRendering() override
        {
            visitor1->beginDegreeRendering();
        }        
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) override
        {
            visitor1->visitDegree(chord, def, degreeEvent);
        }
        virtual void endDegreeRendering() override
        {
            visitor1->endDegreeRendering();
        }
        virtual void visitInstrument(const com::String &uname, const com::String instrumentSectionName) override 
        {
            visitor1->visitInstrument(uname, instrumentSectionName);
        }
        virtual void beginRenderPitch(const documentModel::PitchDef &pitch) override 
        {
            visitor1->beginRenderPitch(pitch);
        }
        virtual void endRenderPitch() override 
        {
            visitor1->endRenderPitch();
        }
        virtual void beginRenderPhrase(const com::String& phraseName) override 
        {
            visitor1->beginRenderPhrase(phraseName);
        }
        virtual void endRenderPhrase(const com::String& phraseName) override 
        {
            visitor1->endRenderPhrase(phraseName);
        }
        virtual void beginRenderTemplate(const com::String& templateName) override 
        {
            visitor1->beginRenderTemplate(templateName);
        }
        virtual void endRenderTemplate() override 
        {
            visitor1->endRenderTemplate();
        }
        virtual void beginRenderVoice(int voiceNumber) override 
        {
            visitor1->beginRenderVoice(voiceNumber);
        }
        virtual void endRenderVoice() override 
        {
            visitor1->endRenderVoice();
        }
        TVisitor1Ptr visitor1 = nullptr;
    };


    template<class TVisitor1, class TVisitor2>
    class CompoundVisitor_N2 : public CompoundVisitor_N1<TVisitor2>
    {
    public:
        typedef CompoundVisitor_N1<TVisitor2> TBase;
        typedef CompoundVisitor_N2<TVisitor1, TVisitor2> TClass;
        typedef std::shared_ptr<TVisitor1> TVisitor1Ptr;
        typedef std::shared_ptr<TVisitor2> TVisitor2Ptr;
        typedef std::shared_ptr<TClass> TSharedPtr;
        CompoundVisitor_N2(TVisitor1Ptr visitor1, TVisitor2Ptr visitor2)
            : TBase(visitor2), visitor1(visitor1)
        {
        }
        virtual ~CompoundVisitor_N2() = default;
        virtual void beginCompile() override
        {
            visitor1->beginCompile();
            TBase::beginCompile();
        }
        virtual void endCompile() override
        {
            visitor1->endCompile();
            TBase::endCompile();
        }
        virtual void visit(IContext* context, const documentModel::Event& ev) override
        {
            visitor1->visit(context, ev);
            TBase::visit(context, ev);
        }
        virtual void visit(IContext* context, const com::midi::Event& ev, IContext::TrackId trackId) override
        {
            visitor1->visit(context, ev, trackId);
            TBase::visit(context, ev, trackId);
        }
        virtual void beginDegreeRendering() override
        {
            visitor1->beginDegreeRendering();
            TBase::beginDegreeRendering();
        }           
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) override
        {
            visitor1->visitDegree(chord, def, degreeEvent);
            TBase::visitDegree(chord, def, degreeEvent);
        }
        virtual void endDegreeRendering() override
        {
            visitor1->endDegreeRendering();
            TBase::endDegreeRendering();
        }
        virtual void visitInstrument(const com::String &uname, const com::String instrumentSectionName) override 
        {
            visitor1->visitInstrument(uname, instrumentSectionName);
            TBase::visitInstrument(uname, instrumentSectionName);
        }
        virtual void beginRenderPitch(const documentModel::PitchDef &pitch) override 
        {
            visitor1->beginRenderPitch(pitch);
            TBase::beginRenderPitch(pitch);
        }
        virtual void endRenderPitch() override 
        {
            visitor1->endRenderPitch();
            TBase::endRenderPitch();
        }
        virtual void beginRenderPhrase(const com::String& phraseName) override 
        {
            visitor1->beginRenderPhrase(phraseName);
            TBase::beginRenderPhrase(phraseName);
        }
        virtual void endRenderPhrase(const com::String& phraseName) override 
        {
            visitor1->endRenderPhrase(phraseName);
            TBase::endRenderPhrase(phraseName);
        }
        virtual void beginRenderTemplate(const com::String& templateName) override 
        {
            visitor1->beginRenderTemplate(templateName);
            TBase::beginRenderTemplate(templateName);
        }
        virtual void endRenderTemplate() override 
        {
            visitor1->endRenderTemplate();
            TBase::endRenderTemplate();
        }
        virtual void beginRenderVoice(int voiceNumber) override 
        {
            visitor1->beginRenderVoice(voiceNumber);
            TBase::beginRenderVoice(voiceNumber);
        }
        virtual void endRenderVoice() override 
        {
            visitor1->endRenderVoice();
            TBase::endRenderVoice();
        }
        TVisitor1Ptr visitor1 = nullptr;
    };
    
}