#include "EventInformationServer.h"
#include <documentModel/objects/Event.h>
#include <compiler/ICompiler.h>
#include <boost/bimap.hpp>
#include <com/midi.hpp>
#include <iostream>
#include <algorithm>
#include <list>
#include <compiler/metaCommands.h>

namespace compiler
{
	using namespace boost;
	using namespace boost::multi_index;
	class EventInformationDb
	{
		struct StringValue {};
		typedef multi_index_container<
			EventInformation,
			indexed_by<
				ordered_unique<member<EventInformation, std::string, &EventInformation::id>>,
				ordered_non_unique<tag<StringValue>, member<EventInformation, com::String, &EventInformation::stringValue> >
			>
		> EventSet;
		EventSet events;
	public:
		inline std::string createId(const documentModel::Event& ev) const
		{
			return std::to_string(ev.sourceId) + "-" + std::to_string(ev.sourcePositionBegin);
		}
		void insert(const documentModel::Event&, const com::midi::Event&);
		void clear();
		std::list<EventInformation> findEventInformationsByStringValue(const com::String&);
	};

	void EventInformationDb::insert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent)
	{
		EventInformation ei;
		ei.eventType = documentEvent.type;
		ei.id = createId(documentEvent);
		ei.stringValue = documentEvent.stringValue;
		ei.absolutePosition = midiEvent.absPosition();
		ei.metaArgs = documentEvent.metaArgs;
		events.insert(ei);
	}

	void EventInformationDb::clear()
	{
		events.clear();
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

	void EventInformationServer::beginCompile()
	{
		// we need to clear here, because this instance exists over the whole lifetime of 
		// an application. It is possible that a document is will be compiled several times (e.g. player in watch mode)
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
		eventDb->insert(*lastDocumentEvent, ev);
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
}
