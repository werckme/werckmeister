#ifndef DEFAULT_COMPILER_VISITOR_H
#define DEFAULT_COMPILER_VISITOR_H


#include "ICompilerVisitor.h"

namespace sheet {
	struct Event;
	namespace compiler {
        class DefaultCompilerVisitor : public ICompilerVisitor {
        public:
            virtual void visit(IContext *context, const sheet::Event &ev) {}
            virtual void visit(IContext *context, const fm::midi::Event &ev, IContext::TrackId trackId) {}
            virtual ~DefaultCompilerVisitor() = default;
        };
    }
}

#endif