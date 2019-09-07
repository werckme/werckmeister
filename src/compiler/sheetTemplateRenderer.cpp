#include "sheetTemplateRenderer.h"
#include "error.hpp"
#include <sheet/tools.h>
#include <iostream>
#include <boost/exception/get_error_info.hpp>
#include "sheetEventRenderer.h"
#include <algorithm>
#include <functional>

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
			class DegreeEventServer {
				const Voice::Events *degrees_;
				Voice::Events::const_iterator it_;
				void degrees(const Voice::Events *degrees);
			public:
				DegreeEventServer(const Voice::Events *degrees);
				const Event * nextEvent();
				bool hasTimeConsumingEvents() const;
				void seek(fm::Ticks);

			};
			DegreeEventServer::DegreeEventServer(const Voice::Events *degrees)
			{
				this->degrees(degrees);
			}
			void DegreeEventServer::degrees(const Voice::Events *degrees)
			{
				this->degrees_ = degrees;
				it_ = this->degrees_->begin();
			}
			const Event * DegreeEventServer::nextEvent()
			{
				if (it_ == degrees_->end()) {
					seek(0);
				}
				return &(*it_++);
			}
			bool DegreeEventServer::hasTimeConsumingEvents() const
			{
				return std::any_of(degrees_->begin(), degrees_->end(), [](const Event &ev) { 
					return ev.isTimeConsuming();
				});
			}
			void DegreeEventServer::seek(fm::Ticks ticks)
			{
				if (ticks != 0) {
					FM_THROW(Exception, "missing implementation for seeking to value != 0");
				}
				it_ = degrees_->begin();
			}			
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
					else {
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

			void __handleTemplatePositionCmd(const Event &metaEvent, DegreeEventServer &eventServer)
			{
				try {
					const auto &args = metaEvent.metaArgs;
					if (args.empty()) {
						FM_THROW(Exception, fm::String("no args for: ") + SHEET_META__SHEET_TEMPLATE_POSITION);
					}
					auto arg = args.front();
					if (arg == SHEET_META__SHEET_TEMPLATE_POSITION_CMD_RESET) {
						eventServer.seek(0);
					} else {
						FM_THROW(Exception, fm::String("invalid arg for: ") + SHEET_META__SHEET_TEMPLATE_POSITION + ": " + arg);
					}
				} catch (fm::Exception &ex) {
					ex << ex_sheet_source_info(metaEvent);
					throw;
				}
			}

			typedef std::list<const Event*> Chords;
			fm::Ticks __renderOneChordBar(AContext *ctx, 
				SheetEventRenderer *sheetEventRenderer, 
				DegreeEventServer &eventServer, 
				const Chords &chords) 
			{
				fm::Ticks leftover = 0;
				fm::Ticks written = 0;
				for(const Event *chord : chords) {
					using namespace fm;
					if (chord->type == Event::Rest) {
						ctx->seek(chord->duration);
						continue;
					}
					if (chord->type == Event::Meta) {
						if (chord->stringValue == SHEET_META__SHEET_TEMPLATE_POSITION) {
							__handleTemplatePositionCmd(*chord, eventServer);
							continue;
						}
						auto voiceId = ctx->voice();
						auto trackId = ctx->track();
						ctx->setChordTrackTarget();
						sheetEventRenderer->addEvent(*chord);
						ctx->setTarget(trackId, voiceId);
						continue;
					}
					fm::Ticks ticksToWrite = chord->duration;
					while(ticksToWrite > 1.0_N128) {
						const Event &degree = *(eventServer.nextEvent());
						Event copy; 
						copy = degree.isRelativeDegree() ? 
							__degreeToAbsoluteNote(ctx, *chord, degree, copy) 
										: degree;
						bool isTimeConsuming = copy.isTimeConsuming();
						if (isTimeConsuming && leftover > 0) {
							copy.duration += leftover;
							leftover = 0;
						}
						if (isTimeConsuming && ticksToWrite - copy.duration < 0) {
							leftover = copy.duration - ticksToWrite;
							copy.duration = ticksToWrite;
						}
						if (copy.type == Event::EOB) {
							// eob will be handled elsewhere
							continue;
						}
						sheetEventRenderer->addEvent(copy);
						if (isTimeConsuming) {
							ticksToWrite -= copy.duration;	
							written += copy.duration;	
						}
					}
				}
				return written;	
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
							DegreeEventServer eventServer(&(voice.events));
							if (!eventServer.hasTimeConsumingEvents()) {
								continue;
							}
							setTargetCreateIfNotExists(*track, voice);
							ctx_->voiceMetaData()->position = previousDuration;
							ctx_->voiceMetaData()->barPosition = 0;
							Chords chordsPerBar;
							for (const auto &chord : templateAndChords.chords)
							{
								if (chord->type == Event::EOB) {
									ctx_->voiceMetaData()->tempoFactor = sheetMeta->tempoFactor;
									__renderOneChordBar(ctx_, sheetEventRenderer, eventServer, chordsPerBar);
									ctx_->newBar();
									chordsPerBar.clear();
									continue;
								}
								if (chord->type == Event::Chord 
									|| chord->type == Event::Meta
									|| chord->type == Event::Rest) 
								{
									chordsPerBar.push_back(chord);
								}
							}
						}
					}
				}
			}
		}
    }
}