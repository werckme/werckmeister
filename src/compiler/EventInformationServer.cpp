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
		struct DocumentId {};
		struct StringValue {};
		typedef multi_index_container<
			EventInformation,
			indexed_by<
				ordered_unique<tag<Id>, member<EventInformation, EventInformation::Id, &EventInformation::id>>,
				ordered_non_unique<tag<DocumentId>, member<EventInformation, com::String, &EventInformation::documentId>>,
				ordered_non_unique<tag<StringValue>, member<EventInformation, com::String, &EventInformation::stringValue>>
			>
		> EventSet;
		typedef std::unordered_map<com::String, com::Ticks> CuePositionMap;
		EventSet events;
		CuePositionMap cuePositionMap;
	private:
		void insert(const documentModel::Event&, const com::midi::Event&, ChordRenderInfoPtr chordRenderInfo, const com::String &instrumentName, const com::String& instrumentSectionName);
		void update(const EventInformation&, const documentModel::Event&, const com::midi::Event&, ChordRenderInfoPtr chordRenderInfo, const com::String& instrumentName, const com::String& instrumentSectionName);
		void updateCueEventMap(const com::midi::Event&);
	public:
		inline EventInformation::DocumentId createDocumentId(const documentModel::Event& ev) const
		{
			return std::to_string(ev.sourceId) + "-" + std::to_string(ev.sourcePositionBegin);
		}
		void upsert(const documentModel::Event&, const com::midi::Event&, ChordRenderInfoPtr chordRenderInfo, const com::String& instrumentName, const com::String& instrumentSectionName);
		const EventInformation* find(const documentModel::Event&);
		const EventInformation* find(const com::midi::Event&);
		const EventInformation* find(const EventInformation::Id &id);
		void clear();
		std::list<EventInformation> findEventInformationsByStringValue(const com::String&);
		com::Ticks findCueEventPositon(const com::String cueName);
	};
	const EventInformation* EventInformationDb::find(const com::midi::Event& event)
	{
		auto inf = events.find(event.id);
		return &(*inf);
	}	
	const EventInformation* EventInformationDb::find(const documentModel::Event& event)
	{
		auto documentId = createDocumentId(event);
		auto range = events.get<DocumentId>().equal_range(documentId);
		if (range.first == range.second) {
			return nullptr;
		}
		return &(*range.first);

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
	void EventInformationDb::insert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, ChordRenderInfoPtr chordRenderInfo, const com::String& instrumentName, const com::String& instrumentSectionName)
	{
		EventInformation ei;
		ei.id = midiEvent.id;
		ei.documentId = createDocumentId(documentEvent);
		ei.eventType = documentEvent.type;
		ei.stringValue = documentEvent.stringValue;
		ei.metaArgs = documentEvent.metaArgs;
		ei.tags = documentEvent.tags;
		ei.chordRenderInfo = chordRenderInfo;
		ei.instrumentName = instrumentName;
		ei.instrumentSectionName = instrumentSectionName;
		events.insert(ei);
	}
	void EventInformationDb::update(const EventInformation& evinf, const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, ChordRenderInfoPtr chordRenderInfo, const com::String& instrumentName, const com::String& instrumentSectionName)
	{
		auto copy = evinf;
		copy.eventType = documentEvent.type;
		copy.stringValue = documentEvent.stringValue;
		copy.metaArgs = documentEvent.metaArgs;
		copy.tags = documentEvent.tags;
		copy.chordRenderInfo = chordRenderInfo;
		copy.instrumentName = instrumentName;
		copy.instrumentSectionName = instrumentSectionName;
		auto it = events.find(copy.id);
		events.replace(it, copy);
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
	void EventInformationDb::upsert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, ChordRenderInfoPtr chordRenderInfo, const com::String& instrumentName, const com::String& instrumentSectionName)
	{
		updateCueEventMap(midiEvent);
		auto infoIt = events.find(midiEvent.id);
		if (infoIt == events.end()) 
		{
			insert(documentEvent, midiEvent, chordRenderInfo, instrumentName, instrumentSectionName);
		} else {
			update(*infoIt, documentEvent, midiEvent, chordRenderInfo, instrumentName, instrumentSectionName);
		}
	}

	void EventInformationDb::clear()
	{
		events.clear();
		cuePositionMap.clear();
	}

	std::list<EventInformation> EventInformationDb::findEventInformationsByStringValue(const com::String& stringValue)
	{
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
		eventDb->upsert(*lastDocumentEvent, ev, lastChordRenderInfo, lastInstrument, lastInstrumentSectionName);
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

	void EventInformationServer::visitInstrument(const com::String& uname, const com::String instrumentSectionName)
	{
		lastInstrument = uname;
		lastInstrumentSectionName = instrumentSectionName;
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

	void EventInformationServer::visitDegree(const documentModel::Event& chord, const documentModel::ChordDef& def, const documentModel::Event& degreeEvent)
	{
		lastChordRenderInfo = std::make_shared<ChordRenderInfo>();
		lastChordRenderInfo->chordDef = def;
		lastChordRenderInfo->chordEvent = chord;
		lastChordRenderInfo->degreeEvent = degreeEvent;
	}

	void EventInformationServer::endDegreeRendering()
	{
		this->lastChordRenderInfo.reset();
	}
}
