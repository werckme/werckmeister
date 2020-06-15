#ifndef DEFAULT_COMPILER_VISITOR_H
#define DEFAULT_COMPILER_VISITOR_H


#include "ICompilerVisitor.h"

namespace sheet {
	struct Event;
	namespace compiler {
        class DefaultCompilerVisitor : public ICompilerVisitor {
        public:
            virtual void visit(const sheet::Event &ev) {}
            virtual void visit(const fm::midi::Event &ev, IContext::TrackId trackId) {}
            virtual ~DefaultCompilerVisitor() = default;
        };
    }
}

#endif