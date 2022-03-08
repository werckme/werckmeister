#pragma once

#include "DefaultCompilerVisitor.h"
#include "IEventInformationServer.h"
#include <memory>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class EventInformationDb;
    class EventInformationServer : public DefaultCompilerVisitor, public IEventInformationServer
    {
    public:
        EventInformationServer();
        virtual void beginCompile() override; 
        virtual void endCompile() override;
        virtual void visit(IContext *context, const documentModel::Event &ev) override;
        virtual void visit(IContext *context, const com::midi::Event &ev, IContext::TrackId trackId) override;
        virtual void visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent) override;
        virtual void endDegreeRendering() override;
        virtual ~EventInformationServer();
        virtual com::Ticks findCueEventPosition(const com::String& cueName) override;
        virtual const EventInformation* find(const documentModel::Event &ev) const override;
        virtual const EventInformation* find(const com::midi::Event &ev) const override;
        virtual Tags getTags(const com::midi::Event &ev) const override;          
    private:
        std::unique_ptr<EventInformationDb> eventDb;
        const documentModel::Event *lastDocumentEvent = nullptr;
        ChordRenderInfoPtr lastChordRenderInfo;

    };
}
