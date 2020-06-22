#ifndef DEFAULT_COMPILER_VISITOR_H
#define DEFAULT_COMPILER_VISITOR_H


#include "ICompilerVisitor.h"

namespace sheet {
	struct Event;
	namespace compiler {
        class DefaultCompilerVisitor : public ICompilerVisitor {
        public:
            virtual void beginCompile() override {}
            virtual void endCompile() override {}
            virtual void visit(IContext *context, const sheet::Event &ev) override {}
            virtual void visit(IContext *context, const fm::midi::Event &ev, IContext::TrackId trackId) override {}
            virtual ~DefaultCompilerVisitor() = default;
        };
    }
}

#endif