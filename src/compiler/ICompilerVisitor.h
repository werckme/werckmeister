#ifndef ICOMPILER_VISITOR_H
#define ICOMPILER_VISITOR_H

namespace fm {
    namespace midi {
        struct Event;
    }
}

#include <compiler/context/IContext.h>

namespace sheet {
	struct Event;
	namespace compiler {
        class ICompilerVisitor {
        public:
            virtual void visit(IContext *context, const sheet::Event &ev) = 0;
            virtual void visit(IContext *context, const fm::midi::Event &ev, IContext::TrackId trackId) = 0;
        };
        typedef std::shared_ptr<ICompilerVisitor> ICompilerVisitorPtr;
    }
}

#endif