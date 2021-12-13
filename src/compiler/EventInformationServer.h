#pragma once

#include "ICompilerVisitor.h"
#include "IEventInformationServer.h"
namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class EventInformationServer : public ICompilerVisitor, public IEventInformationServer
    {
    public:
        virtual void beginCompile() override { /*TODO clear collected informations */ }
        virtual void endCompile() override {}
        virtual void visit(IContext *context, const documentModel::Event &ev) override {}
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override {}
        virtual ~EventInformationServer() = default;
    };
}
