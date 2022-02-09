#include "EventInformationServer.h"
#include <documentModel/objects/Event.h>
#include <compiler/ICompiler.h>
#include <boost/bimap.hpp>
#include <com/midi.hpp>
#include <iostream>
#include <algorithm>
#include <list>
#include <compiler/metaCommands.h>
#include <unordered_map>

namespace compiler
{
	using namespace boost;
	using namespace boost::multi_index;
	class EventInformationDb
	{
		struct Id {};
		struct StringValue {};
		typedef multi_index_container<
			EventInformation,
			indexed_by<
				ordered_unique<tag<Id>, member<EventInformation, std::string, &EventInformation::id>>,
				ordered_non_unique<tag<StringValue>, member<EventInformation, com::String, &EventInformation::stringValue>>
			>
		> EventSet;
		typedef std::unordered_map<com::midi::Event, EventInformation::Id, com::midi::EventHasher> MidiEventMap;
		EventSet events;
		MidiEventMap midiEventMap;
	public:
		inline EventInformation::Id createId(const documentModel::Event& ev) const
		{
			return std::to_string(ev.sourceId) + "-" + std::to_string(ev.sourcePositionBegin);
		}
		void insert(const EventInformation::Id &id, const documentModel::Event&, const com::midi::Event&);
		void upsert(const documentModel::Event&, const com::midi::Event&);
		const EventInformation* find(const documentModel::Event&);
		const EventInformation* find(const com::midi::Event&);
		const EventInformation* find(const EventInformation::Id &id);
		void clear();
		std::list<EventInformation> findEventInformationsByStringValue(const com::String&);
	};
	const EventInformation* EventInformationDb::find(const com::midi::Event& event)
	{
		auto infoIdIt = midiEventMap.find(event);
		if (infoIdIt == midiEventMap.end())
		{
			return nullptr;
		}
		return find(infoIdIt->second);
	}	
	const EventInformation* EventInformationDb::find(const documentModel::Event& event)
	{
		auto id = createId(event);
		return find(id);

	}
	const EventInformation* EventInformationDb::find(const EventInformation::Id &id)
	{
		auto infoIt = events.find(id);
		if (infoIt == events.end()) 
		{
			return nullptr;
		}
		return &(*infoIt);
	}		
	void EventInformationDb::insert(const EventInformation::Id &id, const documentModel::Event& documentEvent, const com::midi::Event& midiEvent)
	{
		EventInformation ei;
		ei.id = id;
		ei.eventType = documentEvent.type;
		ei.stringValue = documentEvent.stringValue;
		ei.positions.push_back(midiEvent.absPosition());
		ei.metaArgs = documentEvent.metaArgs;
		ei.tags = documentEvent.tags;
		events.insert(ei);
	}

	void EventInformationDb::upsert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent)
	{
		auto id = createId(documentEvent);
		midiEventMap.insert({midiEvent, id});
		auto infoIt = events.find(id);
		if (infoIt == events.end()) 
		{
			return insert(id, documentEvent, midiEvent);
		}
		auto info = *infoIt;
		info.positions.push_back(midiEvent.absPosition());
		events.replace(infoIt, info);
	}

	void EventInformationDb::clear()
	{
		events.clear();
		midiEventMap.clear();
	}

	std::list<EventInformation> EventInformationDb::findEventInformationsByStringValue(const com::String& stringValue)
	{
		typedef EventSet::index<StringValue>::type set_by_name;
		auto range = events.get<StringValue>().equal_range(stringValue);
		std::list<EventInformation> result;
		std::copy(range.first, range.second, std::back_inserter(result));
		return result;
	}

	///////////////////////////////////////////////////////////////////////////

	EventInformationServer::EventInformationServer() : eventDb(new EventInformationDb())
	{
	}

	EventInformationServer::~EventInformationServer() = default;

	void EventInformationServer::beginCompile()
	{
		// we need to clear here, because this instance exists over the whole lifetime of 
		// an application. It is possible that a document is will be compiled several times (e.g. player in watch mode)
		lastDocumentEvent = nullptr;
		eventDb->clear();
	}

	void EventInformationServer::endCompile()
	{

	}

	void EventInformationServer::visit(IContext* context, const documentModel::Event& ev)
	{
		lastDocumentEvent = &ev;
	}

	void EventInformationServer::visit(IContext* context, const com::midi::Event& ev, IContext::TrackId trackId)
	{
		bool canProcess = !!lastDocumentEvent
			&& lastDocumentEvent->sourcePositionBegin != documentModel::Event::UndefinedPosition
			&& lastDocumentEvent->sourceId != documentModel::Event::UndefinedSource;
		if (!canProcess)
		{
			return;
		}
		eventDb->upsert(*lastDocumentEvent, ev);
	}

	EventInformationServer::EventInformationSet EventInformationServer::findCueEvents(const com::String& cueName)
	{
		EventInformationSet result;
		auto eventInformations = eventDb->findEventInformationsByStringValue(SHEET_META__ADD_CUE);
		for (const auto& eventInfo : eventInformations)
		{
			bool isInfoOfInterest = eventInfo.eventType == documentModel::Event::Meta;
			if (!isInfoOfInterest)
			{
				continue;
			}
			bool containsCueName = std::find_if(eventInfo.metaArgs.begin(), eventInfo.metaArgs.end(), [cueName](auto x) { return cueName == x.value; }) != eventInfo.metaArgs.end();
			if (!containsCueName) 
			{
				continue;
			}
			result.insert(eventInfo);
		}
		return result;
	}

	const EventInformation* EventInformationServer::find(const documentModel::Event &ev) const
	{
		return eventDb->find(ev);
	}
	const EventInformation* EventInformationServer::find(const com::midi::Event &ev) const
	{
		return eventDb->find(ev);
	}

	IEventInformationServer::Tags EventInformationServer::getTags(const com::midi::Event &ev) const 
	{
		auto *evInformation = find(ev);
		if (!evInformation) 
		{
			return IEventInformationServer::Tags();
		}
		return IEventInformationServer::Tags(evInformation->tags.begin(), evInformation->tags.end());
	}
}
