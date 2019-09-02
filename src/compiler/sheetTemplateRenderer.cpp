#include "sheetTemplateRenderer.h"
#include "error.hpp"
#include <sheet/tools.h>
#include <iostream>
#include <boost/exception/get_error_info.hpp>
#include "sheetEventRenderer.h"
#include <algorithm>
#include <numeric>

#define DEBUGX(x)

namespace sheet {
    namespace compiler {

		SheetTemplateRenderer::SheetTemplateRenderer(AContext* ctx, SheetEventRenderer *renderer) : 
			ctx_(ctx) 
			, sheetEventRenderer(renderer)
		{
		}

		SheetTemplateRenderer::~SheetTemplateRenderer() 
		{
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

		namespace {
			struct TemplatesAndItsChords {
				typedef std::vector<Event*> Chords;
				typedef AContext::SheetTemplates Templates;
				Chords chords;
				Templates templates;
			};
			AContext::SheetTemplates __getTemplates(SheetTemplateRenderer &sheetTemplateRenderer, const Event &metaEvent)
			{
				auto ctx = sheetTemplateRenderer.context();
				AContext::SheetTemplates templates;
				for (size_t idx=0; idx < metaEvent.metaArgs.size(); ++idx) {
					auto sheetTemplateName = getArgument<fm::String>(metaEvent, idx);
					auto sheetTemplate = ctx->sheetTemplateDefServer()->getSheetTemplate(sheetTemplateName);
					if (sheetTemplate.empty()) {
						FM_THROW(Exception, "sheetTemplate not found: " + sheetTemplateName);
					}
					templates.push_back(sheetTemplate);
				}
				return templates;
			}
			std::list<TemplatesAndItsChords> __collectChordsPerTemplate(SheetTemplateRenderer &sheetTemplateRenderer, Track * sheetTrack)
			{
				auto ctx = sheetTemplateRenderer.context();
				std::list<TemplatesAndItsChords> templatesAndItsChords;
				templatesAndItsChords.emplace_back(TemplatesAndItsChords());
				templatesAndItsChords.back().templates = ctx->currentSheetTemplates();

				auto &sheetEvents = sheetTrack->voices.begin()->events;
				for (auto &ev : sheetEvents) {
					if (ev.stringValue == SHEET_META__SET_SHEET_TEMPLATE) {
						templatesAndItsChords.emplace_back(TemplatesAndItsChords());
						templatesAndItsChords.back().templates = __getTemplates(sheetTemplateRenderer, ev);
					}
					if (ev.isTimeConsuming()) {
						templatesAndItsChords.back().chords.push_back(&ev);
					}
				}
				return templatesAndItsChords;
			}

			Event & __degreeToAbsoluteNote(AContext *ctx, const Event &chordEvent, const Event &degreeEvent, Event &target) {
				auto chordDef = ctx->sheetTemplateDefServer()->getChord(chordEvent.chordDefName());
				if (chordDef == nullptr) {
					FM_THROW(Exception, "chord not found: " + chordEvent.stringValue);
				}									
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(chordEvent, *chordDef, degreeEvent.pitches, ctx->getTimeInfo());
				target = degreeEvent;
				target.type = Event::Note;
				target.isTied(degreeEvent.isTied());
				target.pitches.swap(pitches);
				return target;
			}
		}

		void SheetTemplateRenderer::render(Track * sheetTrack)
		{
			auto sheetMeta = ctx_->voiceMetaData(ctx_->chordVoiceId());
			auto templatesAndItsChords = __collectChordsPerTemplate(*this, sheetTrack);
			const TemplatesAndItsChords *previousTemplateAndChords = nullptr;
			fm::Ticks previousDuration = 0;
			for(auto const &templateAndChords : templatesAndItsChords) {
				if (templateAndChords.chords.empty()) {
					continue;
				}
				if (previousTemplateAndChords != nullptr) {
					const auto &chords = previousTemplateAndChords->chords;
					std::for_each(chords.begin(), chords.end(), [&previousDuration](const Event *ev) {
						previousDuration += ev->duration;
					});
				}
				previousTemplateAndChords = &templateAndChords;
				for (const auto &tmpl : templateAndChords.templates) {
					const auto &sheetTemplateTracks = tmpl.tracks;
					for (const auto &track : sheetTemplateTracks)
					{
						for (const auto &voice : track->voices)
						{
							if (voice.events.empty()) {
								continue;
							}
							size_t chordIdx = 0;
							size_t degreeEventIdx = 0;
							fm::Ticks writtenTicks = 0;
							fm::Ticks writtenTicksPerVoice = 0;
							fm::Ticks leftOver = 0;
							setTargetCreateIfNotExists(*track, voice);
							ctx_->voiceMetaData()->position = previousDuration;
							ctx_->voiceMetaData()->barPosition = 0;
							ctx_->voiceMetaData()->tempoFactor = sheetMeta->tempoFactor;
							while(true) { // iterate events
								const Event &degreeEvent = voice.events.at(degreeEventIdx);
								const Event *chordEvent = templateAndChords.chords.at(chordIdx);
								Event copy;
								if(chordEvent->type == Event::Rest) 
								{
									copy = degreeEvent;
									copy.type = Event::Rest;
									copy.duration = chordEvent->duration;
									sheetEventRenderer->addEvent(copy);
									ctx_->newBar();
								} 
								else
								{
									copy = degreeEvent.isRelativeDegree() ? 
										__degreeToAbsoluteNote(ctx_, *chordEvent, degreeEvent, copy) 
										: degreeEvent;
									if (leftOver > 0) {
										copy.duration += leftOver;
									}
									leftOver = writtenTicks + copy.duration - chordEvent->duration;
									if (leftOver > 0) {
										copy.duration -= leftOver;
									}
									sheetEventRenderer->addEvent(copy);
									++degreeEventIdx;
								}  

								writtenTicks += copy.duration * sheetMeta->tempoFactor;
								writtenTicksPerVoice += copy.duration * sheetMeta->tempoFactor;
								
								if (writtenTicks >= chordEvent->duration) {
									if (chordIdx +1 >= templateAndChords.chords.size()) {
										// no more chords
										break;
									}
									// switch to next chord
									++chordIdx;
									writtenTicks = 0;
								}

								if (degreeEventIdx >= voice.events.size()) {
									if (writtenTicksPerVoice==0) {
										// no time consuming events here
										break;
									}
									degreeEventIdx = 0;
									if (copy.type != Event::EOB) {
										// implicite new bar at end of voice
										ctx_->newBar(); 
									}
								}
							}
						}
					}
				}
			}
		}
    }
}