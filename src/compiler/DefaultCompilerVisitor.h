#pragma once


#include "ICompilerVisitor.h"

namespace documentModel {
	struct Event;
	namespace compiler {
        class DefaultCompilerVisitor : public ICompilerVisitor {
        public:
            virtual void beginCompile() override {}
            virtual void endCompile() override {}
            virtual void visit(IContext *context, const documentModel::Event &ev) override {}
            virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override {}
            virtual ~DefaultCompilerVisitor() = default;
        };
    }
}

