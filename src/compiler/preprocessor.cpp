#include "preprocessor.h"
#include <sheet/Track.h>
#include <compiler/metaData.h>
#include "error.hpp"

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
						FM_THROW(Exception, "no event for shortcut: x");
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
				for (; it!=end; ++it)
				{
					auto &ev = *it;
					processEvent(ev, processData);
				}
				if (!processData.hasTimeConsumingEvents) {
					// no need to do anything
					voice.events.clear();
				}
			}
        }

    }
}