#include "Preprocessor.h"
#include <sheet/objects/Track.h>
#include <compiler/metaData.h>
#include "error.hpp"
#include <functional>
#include <sheet/Document.h>
#include "metaCommands.h"
#include <fm/tools.h>
#include <algorithm>

namespace sheet {
	namespace compiler {

		namespace {

			struct ProcessData {
				Event lastNoRepeat;
				bool hasTimeConsumingEvents;
				fm::Ticks lastDuration;
			};

			inline Event::Type resolveRepeatType(const Event &lastNoRepeat, const Event &repeat) 
			{
				if (lastNoRepeat.isRelativeDegree()) {
					return repeat.type == Event::Repeat ? Event::Degree : Event::TiedDegree;
				}
				return repeat.type == Event::Repeat ? Event::Note : Event::TiedNote;
			}

			void processEvent(Event &ev, ProcessData &processData)
			{
				if (!ev.isTimeConsuming()) {
					return;
				}
				processData.hasTimeConsumingEvents = true;
				if (ev.type == Event::Group) {
					for (auto &groupedEvent : ev.eventGroup) {
						processEvent(groupedEvent, processData);
					}
				}
				if (!ev.isRepeat() && ev.type != Event::Rest) {
					processData.lastNoRepeat = ev;
				}	
				if (ev.isRepeat())  {
					if (processData.lastNoRepeat.type == Event::Unknown) {
						FM_THROW(Exception, "no prevoius event for repeat symbol: '&'");
					}
					ev.pitches = processData.lastNoRepeat.pitches;
					ev.type = resolveRepeatType(processData.lastNoRepeat, ev);
				}
				if (ev.duration == 0) {
					ev.duration = processData.lastDuration;
				} else {
					processData.lastDuration = ev.duration;
				}
			}

			template<typename EventIteator>
			void determineTiedDuration(EventIteator eventIt, EventIteator end)
			{
				Event &startEvent = *eventIt;
				if (startEvent.tiedDurationTotal != Event::NoDuration) {
					// already processed
					return;
				}
				std::vector<Event*> totalDurations;
				totalDurations.reserve(end - eventIt);
				startEvent.tiedDurationTotal = startEvent.duration;
				while((++eventIt) != end) {
					Event &event = *eventIt;
					bool isSamePitch = event.pitches.size() >= startEvent.pitches.size() &&
						std::is_permutation(startEvent.pitches.begin(), startEvent.pitches.end(), event.pitches.begin());
					startEvent.tiedDurationTotal += event.duration;
					if (!isSamePitch) {
						continue;
					}
					totalDurations.push_back(&event);
					event.tiedDuration = startEvent.tiedDurationTotal;
					if (!event.isTied()) {
						break;
					}
				}
				for(auto *event : totalDurations) {
					event->tiedDurationTotal = startEvent.tiedDurationTotal;
					event->tiedDuration = event->tiedDurationTotal - (event->tiedDuration - startEvent.duration);
				}
				startEvent.tiedDuration = startEvent.tiedDurationTotal;
			}
		}

        void Preprocessor::process(Track &track)
        {
            for (auto &voice : track.voices)
			{
				if (voice.events.empty()) {
					continue;
				}
				ProcessData processData;
				processData.lastNoRepeat.type = Event::Unknown;
				auto it = voice.events.begin();
				auto end = voice.events.end();
				auto lastEvent = end - 1;
				processData.hasTimeConsumingEvents = false;
				if (lastEvent->type != Event::EOB) {
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
				for (; it!=end; ++it)
				{
					auto &ev = *it;
					processEvent(ev, processData);
				}
				it = voice.events.begin();
				// second pass (repeat symbols are resolved now)
				for (; it!=end; ++it)
				{
					auto &ev = *it;
					if (ev.isTied()) {
						determineTiedDuration(it, end);
					}
				}				
				if (!processData.hasTimeConsumingEvents) {
					// no need to do anything
					voice.events.clear();
				}
			}
        }
	
		void Preprocessor::preprocessChordTrack(Track& sheetTrack)
		{
			using namespace fm;
			auto voice = sheetTrack.voices.begin(); 
			auto it = voice->events.begin();
			auto end = voice->events.end();
			std::list<Event*> barEvents;
			_context->setChordTrackTarget();
			auto processEob = [&]() {
				int nbOfEventsPerBar = barEvents.size();
				if (nbOfEventsPerBar > 0) {
					std::for_each(barEvents.begin(), barEvents.end(), [&](Event *ev) {
						auto barLength = _context->voiceMetaData()->barLength;
						ev->duration = barLength / nbOfEventsPerBar;
					});
				}
				barEvents.clear();
			};
			while (it != end) {
				if (it->type == Event::Meta) {
					if (it->stringValue != SHEET_META__ADD_CUE) {
						_renderer->handleMetaEvent(*it);
					}
				}					
				if (it->type == Event::EOB) {
					processEob();
				}
				if (it->isTimeConsuming()) {
					barEvents.push_back(&(*it));
				}
				++it;
			}
			if (barEvents.empty()) {
				return;
			}
			// add implicit eob
			Event eob;
			eob.type = Event::EOB;
			processEob(); // first process bar events
			voice->events.push_back(eob); // then modify source container
		}

		void Preprocessor::preprocess(DocumentPtr document)
		{

			for (auto &track : document->sheetDef.tracks)
			{
				for (auto& voice : track.voices) {
					_sheetNavigator->processNavigation(voice);
				}
				fm::String type = fm::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
				bool isNoteEventTrack = type.empty();
				bool isTemplateTrack  = type ==  SHEET_META__SET_SHEET_TEMPLATE;
				if (isNoteEventTrack || isTemplateTrack) {
					process(track);
				}
				if (type == SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP) {
					preprocessChordTrack(track);
				}
			}
		}

    }
}