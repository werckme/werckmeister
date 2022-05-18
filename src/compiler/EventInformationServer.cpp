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

namespace 
{
	const documentModel::PitchDef NoPitchDef;
}

namespace compiler
{
	using namespace boost;
	using namespace boost::multi_index;
	struct AdditionalEventInfos 
	{
		ChordRenderInfoPtr chordRenderInfo; 
		const com::String *instrumentName; 
		const com::String *instrumentSectionName;
		com::Expression expression;
		com::Ticks barPositionQuarters = -1;
		documentModel::PitchDef pitchDef;
	};
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
		void insert(const documentModel::Event&, const com::midi::Event&, const AdditionalEventInfos& additonalEventInfos);
		void update(const EventInformation&, const documentModel::Event&, const com::midi::Event& midiEvent, const AdditionalEventInfos& additonalEventInfos);
		void updateCueEventMap(const com::midi::Event&);
	public:
		inline EventInformation::DocumentId createDocumentId(const documentModel::Event& ev) const
		{
			return std::to_string(ev.sourceId) + "-" + std::to_string(ev.sourcePositionBegin);
		}
		void upsert(const documentModel::Event&, const com::midi::Event&, const AdditionalEventInfos& additonalEventInfos);
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
		if (inf == events.end()) {
			return nullptr;
		}		
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
	void EventInformationDb::insert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, const AdditionalEventInfos& additonalEventInfos)
	{
		EventInformation ei;
		ei.id = midiEvent.id;
		ei.documentId = createDocumentId(documentEvent);
		ei.eventType = documentEvent.type;
		ei.stringValue = documentEvent.stringValue;
		ei.metaArgs = documentEvent.metaArgs;
		ei.tags = documentEvent.tags;
		ei.chordRenderInfo = additonalEventInfos.chordRenderInfo;
		ei.instrumentName = *additonalEventInfos.instrumentName;
		ei.instrumentSectionName = *additonalEventInfos.instrumentSectionName;
		ei.expression = additonalEventInfos.expression;
		ei.barPositionQuarters = additonalEventInfos.barPositionQuarters;
		ei.sourcePositionBegin = documentEvent.sourcePositionBegin;
		ei.sourcePositionEnd = documentEvent.sourcePositionEnd;
		ei.sourceId = documentEvent.sourceId;
		ei.pitchAlias = additonalEventInfos.pitchDef != NoPitchDef ? additonalEventInfos.pitchDef.alias : "";
		events.insert(ei);
	}
	void EventInformationDb::update(const EventInformation& evinf, const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, const AdditionalEventInfos& additonalEventInfos)
	{
		auto copy = evinf;
		copy.eventType = documentEvent.type;
		copy.stringValue = documentEvent.stringValue;
		copy.metaArgs = documentEvent.metaArgs;
		copy.tags = documentEvent.tags;
		copy.chordRenderInfo = additonalEventInfos.chordRenderInfo;
		copy.instrumentName = *additonalEventInfos.instrumentName;
		copy.instrumentSectionName = *additonalEventInfos.instrumentSectionName;
		copy.expression = additonalEventInfos.expression;
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
	void EventInformationDb::upsert(const documentModel::Event& documentEvent, const com::midi::Event& midiEvent, const AdditionalEventInfos& additonalEventInfos)
	{
		updateCueEventMap(midiEvent);
		auto infoIt = events.find(midiEvent.id);
		if (infoIt == events.end()) 
		{
			insert(documentEvent, midiEvent, additonalEventInfos);
		} else 
		{
			update(*infoIt, documentEvent, midiEvent, additonalEventInfos);
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

	void EventInformationServer::beginRenderPitch(const documentModel::PitchDef &pitch)
	{
		lastPitch = pitch;
	}

	void EventInformationServer::endRenderPitch() 
	{
		lastPitch = NoPitchDef;
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
		const auto contextMeta = context->voiceMetaData();
		AdditionalEventInfos additionalEventInfos;
		additionalEventInfos.expression = contextMeta ? contextMeta->expression : com::expression::Undefined;
		additionalEventInfos.chordRenderInfo = lastChordRenderInfo;
		additionalEventInfos.instrumentName = &lastInstrument;
		additionalEventInfos.instrumentSectionName = &lastInstrumentSectionName;
		additionalEventInfos.barPositionQuarters = contextMeta ? contextMeta->barPosition / com::PPQ : -1;
		additionalEventInfos.pitchDef = lastPitch;
		eventDb->upsert(*lastDocumentEvent, ev, additionalEventInfos);
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
