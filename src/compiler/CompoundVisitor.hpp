#pragma once

#include <compiler/ICompilerVisitor.h>

namespace compiler
{
    template<class TVistor1>
    class CompoundVisitor_N1 : public ICompilerVisitor
    {
    public:
        typedef CompoundVisitor_N1<TVistor1> TClass;
        typedef std::shared_ptr<TVistor1> TVistor1Ptr;
        typedef std::shared_ptr<TClass> TSharedPtr;
        CompoundVisitor_N1(TVistor1Ptr visitor1) 
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
        TVistor1Ptr visitor1 = nullptr;
    };
    
}