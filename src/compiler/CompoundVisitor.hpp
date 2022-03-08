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
        virtual void visitDegree(const documentModel::Event &degreeEvent) override
        {
            visitor1->visitDegree(degreeEvent);
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
        virtual void visitDegree(const documentModel::Event &degreeEvent) override
        {
            visitor1->visitDegree(degreeEvent);
            TBase::visitDegree(degreeEvent);
        }        
        TVisitor1Ptr visitor1 = nullptr;
    };
    
}