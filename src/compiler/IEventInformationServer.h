#pragma once

#include <memory>
#include <set>
#include <documentModel/objects/Event.h>

namespace compiler
{
    struct EventInformation
    {
        documentModel::Event::Type eventType = documentModel::Event::Unknown;
        documentModel::Event::Args metaArgs;
        com::String id;
        com::String stringValue;
        com::Ticks absolutePosition = 0;
        inline bool operator<(const EventInformation& ev) const { return id < ev.id; }
    };

    class IEventInformationServer
    {
    public:
        typedef std::set<EventInformation> EventInformationSet;
        virtual ~IEventInformationServer() = default;
        virtual EventInformationSet findCueEvents(const com::String& cueName) = 0;

    };
    typedef std::shared_ptr<IEventInformationServer> IEventInformationServerPtr;
}
