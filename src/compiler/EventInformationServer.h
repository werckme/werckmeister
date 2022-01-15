#pragma once

#include "ICompilerVisitor.h"
#include "IEventInformationServer.h"
#include <memory>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class EventInformationDb;
    class EventInformationServer : public ICompilerVisitor, public IEventInformationServer
    {
    public:
        EventInformationServer();
        virtual void beginCompile() override; 
        virtual void endCompile() override;
        virtual void visit(IContext *context, const documentModel::Event &ev) override;
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override;
        virtual ~EventInformationServer();
        virtual EventInformationSet findCueEvents(const com::String& cueName) override;
        virtual const EventInformation* find(const documentModel::Event &ev) const override;
        virtual const EventInformation* find(const com::midi::Event &ev) const override;
        virtual Tags getTags(const com::midi::Event &ev) const override;          
    private:
        std::unique_ptr<EventInformationDb> eventDb;
        const documentModel::Event *lastDocumentEvent = nullptr;
    };
}
