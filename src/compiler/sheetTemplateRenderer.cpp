#include "sheetTemplateRenderer.h"
#include "error.hpp"
#include <sheet/tools.h>
#include <iostream>
#include <boost/exception/get_error_info.hpp>

#define DEBUGX(x)

namespace sheet {
	namespace {
		template <class EventContainer>
		int _getIndexOf(const EventContainer &events, fm::Ticks absPosition)
		{
			fm::Ticks pos = 0;
			int idx = -1;
			for (const auto &ev : events) {
				++idx;
				if (pos >= absPosition) {
					return idx;
				}
				pos += ev.duration; 
			}
			return -1;
		}
	}
    namespace compiler {

		void SheetTemplateRenderer::switchSheetTemplate(const ISheetTemplateDefServer::SheetTemplate &current, const ISheetTemplateDefServer::SheetTemplate &next)
		{
			if (next.empty() || current == next) {
				return;
			}
			// set position for new track
			auto chordMeta = ctx_->voiceMetaData();
			for (auto track : next.tracks) {
				for (const auto &voice : track->voices)
				{
					setTargetCreateIfNotExists(*track, voice);
					auto meta = ctx_->voiceMetaData();
					meta->position = chordMeta->position;
				}
			}
			ctx_->currentSheetTemplate(next);
		}

		void SheetTemplateRenderer::seekTo(double quarterNotes)
		{
			if (quarterNotes != 0) {
				FM_THROW(Exception, "only seeking to 0 is supported");
			}
			const auto &sheetTemplateTracks = ctx_->currentSheetTemplate().tracks;
			for (const auto &track : sheetTemplateTracks)
			{
				for (const auto &voice : track->voices)
				{
					if (voice.events.empty() || !hasAnyTimeConsumingEvents(voice.events)) {
						continue;
					}
					auto it = ptrIdMap_.find(&voice);
					if (it == ptrIdMap_.end()) {
						continue;
					}
					auto meta = ctx_->voiceMetaData(it->second);
					meta->lastEventDuration = 0;
					meta->idxLastWrittenEvent = -1;
				}
			}
		}

        void SheetTemplateRenderer::setTargetCreateIfNotExists(const Track &track, const Voice &voice)
		{
			AContext::TrackId trackId;
			AContext::VoiceId voiceId;
			auto it = ptrIdMap_.find(&track);
			bool trackIsNew = false;
			if (it == ptrIdMap_.end()) {
				trackId = ctx_->createTrack();
				ptrIdMap_[&track] = trackId;
				trackIsNew = true;
			}
			else {
				trackId = it->second;
			}
			it = ptrIdMap_.find(&voice);
			if (it == ptrIdMap_.end()) {
				voiceId = ctx_->createVoice();
				ptrIdMap_[&voice] = voiceId;
			}
			else {
				voiceId = it->second;
			}
			ctx_->setTarget(trackId, voiceId);
			if (trackIsNew) {
				try {
					ctx_->processMeta(track.trackInfos, 
						[](const auto &x) { return x.name; }, 
						[](const auto &x) { return x.args; }
					);
				} catch (fm::Exception &ex) {
					if (int *objectIdx = boost::get_error_info<ex_at_object_idx>(ex)) {
						// exception has index on which object the exception occured
						ex << ex_sheet_source_info(track.trackInfos[*objectIdx]);
					}
					throw;
				}
			}
		}

        void SheetTemplateRenderer::sheetRest(fm::Ticks duration)
		{
			const auto &sheetTemplateTracks = ctx_->currentSheetTemplate().tracks;

			for (const auto &track : sheetTemplateTracks)
			{
				for (const auto &voice : track->voices)
				{
					setTargetCreateIfNotExists(*track, voice);
					auto meta = ctx_->voiceMetaData();
					ctx_->rest(duration);
				}
			}
		}

		void SheetTemplateRenderer::remberPosition(const Voice &voice, 
			const Event &ev, 
			Voice::Events::const_iterator it,
			fm::Ticks originalEventDuration)
		{
			auto meta = ctx_->voiceMetaData();
			bool hasRemainings = ev.duration < originalEventDuration;
			if (hasRemainings) {
				meta->remainingTime += originalEventDuration - ev.duration;
			}
			meta->idxLastWrittenEvent = it - voice.events.begin() + 1;
		}

		Voice::Events::const_iterator SheetTemplateRenderer::skipEvents(Voice::Events::const_iterator it, Voice::Events::const_iterator end, int n)
		{
			auto original = it;
			it += n;
			if (it->type == Event::EOB) {
				// happens: | r1 |  ->  | A B |
				// after a half rest the next event would be a new bar
				// its length check would produce a message
				auto meta = ctx_->voiceMetaData();
				
					++it;
					meta->barPosition = 0;
					meta->remainingTime = 0;
					DEBUGX(std::cout << "bar reset" << std::endl);
			}
			if (it >= end) {
				return original;
			}
			return it;
		}

		Voice::Events::const_iterator SheetTemplateRenderer::continueOnRemeberedPosition(const Voice &voice)
		{
			auto it = voice.events.begin();
			auto meta = ctx_->voiceMetaData();
			it = skipEvents(it, voice.events.end(), meta->idxLastWrittenEvent);
			meta->idxLastWrittenEvent = -1;
			return it;
		}

		fm::Ticks SheetTemplateRenderer::renderVoice(const Voice &voice, 
			Voice::Events::const_iterator it, 
			fm::Ticks duration,
			fm::Ticks written)
		{
			DEBUGX(static int c = 10);
			DEBUGX(std::cout << "-----------------------------------------------------" << std::endl);
			DEBUGX(std::cout << "c \t|\t cPos" << "\t|\t" << "bPos" << "\t|\t" << "d" << std::endl);
			DEBUGX(std::cout << "-----------------------------------------------------" << std::endl);
			auto meta = ctx_->voiceMetaData();
			for (; it < voice.events.end(); ++it) // loop voice events
			{
				bool isLastEvent = (it+1) == voice.events.end();
				auto ev = *it;
				auto currentPos = meta->position;
				auto originalDuration = ctx_->getImlplicitDuration(ev);
				if (ev.isTimeConsuming()) {
					ev.duration = originalDuration;
				}
				if (ev.isTimeConsuming() && meta->remainingTime > 0) {
					ev.duration = ev.duration + meta->remainingTime;
					ev.duration = std::min(ev.duration, duration - written);
					// still remainings? keep it
					meta->remainingTime = std::max(fm::Ticks(0.0), meta->remainingTime - (ev.duration - originalDuration));
				} else {
					ev.duration = std::min(ev.duration, duration - written);
				}
				DEBUGX(std::cout << c++ << "," << it - voice.events.begin() << "\t|\t" << currentPos << "\t|\t" << meta->barPosition << "\t|\t" << ev.toString() << ":" << meta->lastEventDuration << std::endl);
				ctx_->addEvent(ev);
				if (ev.isTimeConsuming()) {
					meta->lastEventDuration = originalDuration;
				}
				written += meta->position - currentPos;
				if (allWritten(duration, written) && !isLastEvent) {
					DEBUGX(std::cout << "full" << std::endl);
					remberPosition(voice, ev, it, originalDuration);
					break;
				}
			}
			return written;
		}

        void SheetTemplateRenderer::render(fm::Ticks duration)
        {
            const auto &sheetTemplateTracks = ctx_->currentSheetTemplate().tracks;

			for (const auto &track : sheetTemplateTracks)
			{
				for (const auto &voice : track->voices)
				{
					if (voice.events.empty() || !hasAnyTimeConsumingEvents(voice.events)) {
						continue;
					}
					setTargetCreateIfNotExists(*track, voice);
					auto meta = ctx_->voiceMetaData();
					fm::Ticks writtenDuration = 0;
					while (!allWritten(duration, writtenDuration)) { // loop until enough events are written
						auto it = voice.events.begin();
						if (hasRemberedPosition(*meta)) { // continue rendering
							it = continueOnRemeberedPosition(voice);
						}
						else if (meta->eventOffset > 0) { // skip events (for upbeat)
							it += meta->eventOffset;
						}
						writtenDuration = renderVoice(voice, it, duration, writtenDuration);
					}
				}
			}
        }
    }
}