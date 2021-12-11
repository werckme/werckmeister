#pragma once

namespace com {
    namespace midi {
        struct Event;
    }
}

#include <compiler/context/IContext.h>

namespace documentModel {
	struct Event;
	namespace compiler {
        class ICompilerVisitor {
        public:
            virtual void beginCompile() = 0;
            virtual void endCompile() = 0;
            virtual void visit(IContext *context, const documentModel::Event &ev) = 0;
            virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) = 0;
        };
        typedef std::shared_ptr<ICompilerVisitor> ICompilerVisitorPtr;
    }
}

