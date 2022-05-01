#pragma once

#include <memory>
#include <set>
#include <vector>
#include <documentModel/objects/Event.h>
#include <documentModel/objects/ChordDef.h>

namespace com
{
    namespace midi 
    {
        struct Event;    
    }
}

namespace compiler
{
    struct ChordRenderInfo {
        documentModel::Event chordEvent;
        documentModel::Event degreeEvent;
        documentModel::ChordDef chordDef;
    };
    typedef std::shared_ptr<ChordRenderInfo> ChordRenderInfoPtr;

    struct EventInformation
    {
        typedef unsigned int Id;
        typedef com::String DocumentId;
        documentModel::Event::Type eventType = documentModel::Event::Unknown;
        documentModel::Event::Args metaArgs;
        documentModel::Event::Tags tags;
        ChordRenderInfoPtr chordRenderInfo;
        Id id = -1;
        DocumentId documentId;
        com::String stringValue;
        com::String instrumentName;
        com::String instrumentSectionName;
        com::Expression expression;
        com::Ticks barPositionQuarters = -1;
        int barNumber = -1;
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
