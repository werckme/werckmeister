#pragma once

#include <memory>
#include <set>
#include <vector>
#include <documentModel/objects/Event.h>

namespace com
{
    namespace midi 
    {
        struct Event;    
    }
}

namespace compiler
{
    struct EventInformation
    {
        typedef com::String Id;
        documentModel::Event::Type eventType = documentModel::Event::Unknown;
        documentModel::Event::Args metaArgs;
        documentModel::Event::Tags tags;
        documentModel::Event::Pitches degreeInfos;
        Id id;
        com::String stringValue;
        /**
         * @brief an event can have several positions, in case of repeats and jumps
         * 
         */
        std::vector<com::Ticks> positions;
        inline bool operator<(const EventInformation& ev) const { return id < ev.id; }
    };

    class IEventInformationServer
    {
    public:
        typedef std::vector<com::String> Tags;
        typedef std::set<EventInformation> EventInformationSet;
        virtual ~IEventInformationServer() = default;
        virtual Tags getTags(const com::midi::Event &ev) const = 0;
        /**
            @return -1 if not found
        */
        virtual com::Ticks findCueEventPosition(const com::String& cueName) = 0;
        /**
         * @return nullptr if not found 
         */
        virtual const EventInformation* find(const documentModel::Event &ev) const = 0;
        /**
         * @return nullptr if not found 
         */
        virtual const EventInformation* find(const com::midi::Event &ev) const = 0;        
    };
    typedef std::shared_ptr<IEventInformationServer> IEventInformationServerPtr;
}
