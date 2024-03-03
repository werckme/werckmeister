#include "Preprocessor.h"
#include <documentModel/objects/Track.h>
#include <compiler/metaData.h>
#include "error.hpp"
#include <functional>
#include <documentModel/Document.h>
#include "metaCommands.h"
#include <com/tools.h>
#include <algorithm>
#include <list>

namespace compiler
{

	namespace
	{

		struct ProcessData
		{
			documentModel::Event lastNoRepeat;
			std::list<com::String> tags;
			bool hasTimeConsumingEvents;
			com::Ticks lastDuration;
		};

		inline documentModel::Event::Type resolveRepeatType(const documentModel::Event &lastNoRepeat, const documentModel::Event &repeat)
		{
			using namespace documentModel;
			if (lastNoRepeat.isRelativeDegree())
			{
				return repeat.type == Event::Repeat ? Event::Degree : Event::TiedDegree;
			}
			if(lastNoRepeat.isPhrase()) 
			{
				return repeat.type == Event::Repeat ? Event::Phrase : Event::TiedPhrase;
			}
			if(lastNoRepeat.isGroup()) 
			{
				return Event::Group;
			}
			return repeat.type == Event::Repeat ? Event::Note : Event::TiedNote;
		}

		void processEvent(documentModel::Event &ev, ProcessData &processData)
		{
			using namespace documentModel;
			if (!ev.isTimeConsuming())
			{
				return;
			}
			processData.hasTimeConsumingEvents = true;
			if (ev.type == Event::Group)
			{
				std::list<com::String> tagsCopy(processData.tags.begin(), processData.tags.end());
				processData.tags.insert(processData.tags.begin(), ev.tags.begin(), ev.tags.end());
				for (auto &groupedEvent : ev.eventGroup)
				{
					groupedEvent.tags.insert(processData.tags.begin(), processData.tags.end());
					processEvent(groupedEvent, processData);
				}
				processData.tags.swap(tagsCopy);
			}
			if (!ev.isRepeat() && ev.type != Event::Rest)
			{
				processData.lastNoRepeat = ev;
			}
			if (ev.isRepeat())
			{
				if (processData.lastNoRepeat.type == Event::Unknown)
				{
					FM_THROW(Exception, "no prevoius event for repeat symbol: '&'");
				}
				if (processData.lastNoRepeat.isGroup())
				{
					ev.eventGroup = processData.lastNoRepeat.eventGroup;
				}
				else if (processData.lastNoRepeat.isPhrase())
				{
					ev.stringValue = processData.lastNoRepeat.stringValue;
				}
				else 
				{
					ev.pitches = processData.lastNoRepeat.pitches;
				}
				ev.type = resolveRepeatType(processData.lastNoRepeat, ev);
			}
			if (ev.duration == 0)
			{
				ev.duration = processData.lastDuration;
			}
			else
			{
				processData.lastDuration = ev.duration;
			}
		}

		template <typename EventIteator>
		void determineTiedDuration(EventIteator eventIt, EventIteator end)
		{
			using namespace documentModel;
			Event &startEvent = *eventIt;
			if (startEvent.tiedDurationTotal != Event::NoDuration)
			{
				// already processed
				return;
			}
			std::vector<Event *> totalDurations;
			totalDurations.reserve(end - eventIt);
			startEvent.tiedDurationTotal = startEvent.duration;
			while ((++eventIt) != end)
			{
				Event &event = *eventIt;
				bool isSamePitch = event.pitches.size() >= startEvent.pitches.size() &&
								   std::is_permutation(startEvent.pitches.begin(), startEvent.pitches.end(), event.pitches.begin());
				startEvent.tiedDurationTotal += event.duration;
				if (!isSamePitch)
				{
					continue;
				}
				totalDurations.push_back(&event);
				event.tiedDuration = startEvent.tiedDurationTotal;
				if (!event.isTied() && event.type != Event::EOB)
				{
					break;
				}
			}
			for (auto *event : totalDurations)
			{
				event->tiedDurationTotal = startEvent.tiedDurationTotal;
				event->tiedDuration = event->tiedDurationTotal - (event->tiedDuration - startEvent.duration);
			}
			startEvent.tiedDuration = startEvent.tiedDurationTotal;
		}
	}

	void Preprocessor::process(documentModel::Track &track)
	{
		using namespace documentModel;
		for (auto &voice : track.voices)
		{
			if (voice.events.empty())
			{
				continue;
			}
			ProcessData processData;
			processData.lastNoRepeat.type = Event::Unknown;
			auto it = voice.events.begin();
			auto end = voice.events.end();
			auto lastEvent = end - 1;
			processData.hasTimeConsumingEvents = false;
			if (lastEvent->type != Event::EOB)
			{
				Event eob = *lastEvent;
				eob.duration = 0;
				eob.pitches.clear();
				eob.type = Event::EOB;
				voice.events.push_back(eob);
				it = voice.events.begin();
				end = voice.events.end();
				lastEvent = end - 1;
			}
			processData.lastDuration = VoiceMetaData::DefaultDuration;
			// first pass
			for (; it != end; ++it)
			{
				auto &ev = *it;
				processEvent(ev, processData);
			}
			it = voice.events.begin();
			// second pass (repeat symbols are resolved now)
			for (; it != end; ++it)
			{
				auto &ev = *it;
				if (ev.isTied())
				{
					determineTiedDuration(it, end);
				}
			}
			if (!processData.hasTimeConsumingEvents)
			{
				// no need to do anything
				voice.events.clear();
			}
		}
	}

	void Preprocessor::preprocessChordTrack(documentModel::Track &sheetTrack)
	{
		using namespace documentModel;
		using namespace com;
		auto voice = sheetTrack.voices.begin();
		auto it = voice->events.begin();
		auto end = voice->events.end();
		std::list<Event *> barEvents;
		_context->setChordTrackTarget();
		auto processEob = [&]()
		{
			int nbOfEventsPerBar = barEvents.size();
			if (nbOfEventsPerBar > 0)
			{
				std::for_each(barEvents.begin(), barEvents.end(), [&](Event *ev)
							  {
								  auto barLength = _context->voiceMetaData()->barLength;
								  ev->duration = barLength / nbOfEventsPerBar;
							  });
			}
			barEvents.clear();
		};
		while (it != end)
		{
			if (it->type == Event::Meta)
			{
				if (it->stringValue != SHEET_META__ADD_CUE)
				{
					_renderer->handleMetaEvent(*it);
				}
			}
			if (it->type == Event::EOB)
			{
				processEob();
			}
			if (it->isTimeConsuming())
			{
				barEvents.push_back(&(*it));
			}
			++it;
		}
		if (barEvents.empty())
		{
			return;
		}
		// add implicit eob
		Event eob;
		eob.type = Event::EOB;
		processEob();				  // first process bar events
		voice->events.push_back(eob); // then modify source container
	}

	void Preprocessor::preprocessPhraseDefs(documentModel::DocumentConfig::Events &events)
	{
		documentModel::Track track;
		documentModel::Voice voice;
		if (events.empty()) 
		{
			return;
		}
		for(const auto &event : events)
		{
			try 
			{
				if (event.type == documentModel::Event::EOB)
				{
					FM_THROW(Exception, "bars are not allowed in a phrase");
				}
			}
			catch (const com::Exception &ex)
			{
				ex << compiler::ex_sheet_source_info(event);
				throw;
			}
		}
		voice.events.swap(events);
		track.voices.emplace_back(voice);
		process(track);
		events.swap(track.voices[0].events);
		events.erase(events.end() - 1); // remove last eob
	}

	void Preprocessor::preprocess(documentModel::DocumentPtr document)
	{

		for (auto &track : document->sheetDef.tracks)
		{
			for (auto &voice : track.voices)
			{
				_sheetNavigator->processNavigation(voice);
			}
			bool isNoteEventTrack = com::isNoteEventTrack(track);
			bool isTemplateTrack = com::isTemplateTrack(track);
			if (isNoteEventTrack || isTemplateTrack)
			{
				process(track);
			}
			if (com::isAccompEventTrack(track))
			{
				preprocessChordTrack(track);
			}
		}
		for (auto &documentConfig : document->sheetDef.documentConfigs)
		{
			if (documentConfig.type != documentModel::DocumentConfig::TypePhraseDef) 
			{
				continue;
			}
			preprocessPhraseDefs(documentConfig.events);
		}
	}


}