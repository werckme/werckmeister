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
		typedef std::unordered_map<com::String, com::Ticks> CuePositionMap;
		EventSet events;
		MidiEventMap midiEventMap;
		CuePositionMap cuePositionMap;
	private:
		void insert(const EventInformation::Id& id, const documentModel::Event&, const com::midi::Event&);
		void updateCueEventMap(const com::midi::Event&);
	public:
		inline EventInformation::Id createId(const documentModel::Event& ev) const
		{
			return std::to_string(ev.sourceId) + "-" + std::to_string(ev.sourcePositionBegin);
		}
		void upsert(const documentModel::Event&, const com::midi::Event&);
		const EventInformation* find(const documentModel::Event&);
		const EventInformation* find(const com::midi::Event&);
		const EventInformation* find(const EventInformation::Id &id);
		void clear();
		std::list<EventInformation> findEventInformationsByStringValue(const com::String&);
		com::Ticks findCueEventPositon(const com::String cueName);
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
	void EventInformationDb::updateCueEventMap(const com::midi::Event& midiEvent)
	{
		if (midiEvent.eventType() != com::midi::MetaEvent || midiEvent.metaEventType() != com::midi::CuePoint) 
		{
			return;
		}
		auto name = com::midi::Event::MetaGetStringValue(midiEvent.metaData(), midiEvent.metaDataSize());
		cuePositionMap[name] = midiEvent.absPosition();
	}
	void EventInformationDb::upsert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent)
	{
		updateCueEventMap(midiEvent);
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

	com::Ticks EventInformationDb::findCueEventPositon(const com::String cueName)
	{
		auto cueMapIt = cuePositionMap.find(cueName);
		if (cueMapIt == cuePositionMap.end()) 
		{
			return UndefinedTicks;
		}
		return cueMapIt->second;
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
		if (ev.eventType() == com::midi::MetaEvent) {
			int halt = 0;
		}
		bool canProcess = !!lastDocumentEvent
			&& lastDocumentEvent->sourcePositionBegin != documentModel::Event::UndefinedPosition
			&& lastDocumentEvent->sourceId != documentModel::Event::UndefinedSource;
		if (!canProcess)
		{
			return;
		}
		eventDb->upsert(*lastDocumentEvent, ev);
	}

	com::Ticks EventInformationServer::findCueEventPosition(const com::String& cueName)
	{
		return eventDb->findCueEventPositon(cueName);
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
