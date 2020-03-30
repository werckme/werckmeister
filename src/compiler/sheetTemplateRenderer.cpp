#include "sheetTemplateRenderer.h"
#include "error.hpp"
#include <sheet/tools.h>
#include <iostream>
#include <boost/exception/get_error_info.hpp>
#include "sheetEventRenderer.h"
#include <algorithm>
#include <functional>
#include <fm/werckmeister.hpp>

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
				fm::Ticks offset = 0;
				double tempoFactor = 1;
			};
			class DegreeEventServer {
				const Voice::Events *degrees_;
				Voice::Events::const_iterator it_;
				void degrees(const Voice::Events *degrees);
			public:
				typedef std::function<void()> OnLoop;
				OnLoop onLoop;			
				DegreeEventServer(const Voice::Events *degrees);
				const Event * nextEvent();
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
			void DegreeEventServer::seek(fm::Ticks ticks)
			{
				if (ticks != 0) {
					FM_THROW(Exception, "missing implementation for seeking to value != 0");
				}
				if (onLoop) {
					onLoop();
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
				auto &wm = fm::getWerckmeister();
				auto tmpContext = wm.createTempContext();
				tmpContext->masterTempo(ctx->masterTempo());
				tmpContext->setChordTrackTarget();
				auto &sheetEvents = sheetTrack->voices.begin()->events;
				for (auto &ev : sheetEvents) {
					try {
						bool isTempoEvent = ev.stringValue == SHEET_META__SET_TEMPO;
						bool isTemplateEvent = ev.stringValue == SHEET_META__SET_SHEET_TEMPLATE;
						if (isTemplateEvent) {
							// new template
							templatesAndItsChords.emplace_back(TemplatesAndItsChords());
							auto &newTemplateAndChords = templatesAndItsChords.back();
							newTemplateAndChords.templates = __getTemplates(sheetTemplateRenderer, ev);
							newTemplateAndChords.offset = tmpContext->currentPosition();
							newTemplateAndChords.tempoFactor = tmpContext->voiceMetaData()->tempoFactor;
							
						}
						else {
							// add any other event
							auto &currentTemplateAndChords = templatesAndItsChords.back();
							currentTemplateAndChords.chords.push_back(&ev);
							if (ev.type == Event::Meta) {
								tmpContext->setMeta(ev);
								if (isTempoEvent) {
									templatesAndItsChords.emplace_back(TemplatesAndItsChords());
									auto &newTemplateAndChords = templatesAndItsChords.back();
									newTemplateAndChords.templates = currentTemplateAndChords.templates;								
									newTemplateAndChords.tempoFactor = tmpContext->voiceMetaData()->tempoFactor;
									newTemplateAndChords.offset = tmpContext->currentPosition();
								}
							}
							if (ev.isTimeConsuming()) {
								tmpContext->seek(ev.duration);
							}
						}
					} catch(const Exception &ex) {
						ex << ex_sheet_source_info(ev);
						throw;
					}
				}
				return templatesAndItsChords;
			}

			Event & __degreeToAbsoluteNote(AContext *ctx, const Event &chordEvent, const Event &degreeEvent, Event &target) {
				target = degreeEvent;
				if (degreeEvent.type == Event::Group) {
					target.type = Event::Group;
					size_t index = 0;
					for (const auto &groupedDegreeEvent : degreeEvent.eventGroup) {
						__degreeToAbsoluteNote(ctx, chordEvent, groupedDegreeEvent, target.eventGroup[index++]);
					}
					return target;
				}				
				if (!degreeEvent.isRelativeDegree()) {
					return target;
				}				
				auto chordDef = ctx->sheetTemplateDefServer()->getChord(chordEvent.chordDefName());
				if (chordDef == nullptr) {
					FM_THROW(Exception, "chord not found: " + chordEvent.stringValue);
				}									
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(chordEvent, *chordDef, degreeEvent.pitches, ctx->getTimeInfo());
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

			void __handleChordMeta(AContext *ctx,
			const Event &metaEvent, 
			SheetEventRenderer *sheetEventRenderer,
			DegreeEventServer &eventServer)
			{
				if (metaEvent.stringValue == SHEET_META__SHEET_TEMPLATE_POSITION) {
					__handleTemplatePositionCmd(metaEvent, eventServer);
					return;
				}
				auto voiceId = ctx->voice();
				auto trackId = ctx->track();
				ctx->setChordTrackTarget();
				sheetEventRenderer->addEvent(metaEvent);
				ctx->setTarget(trackId, voiceId);
			}		

			typedef std::list<const Event*> Chords;
			fm::Ticks __renderOneBar(AContext *ctx, 
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
						__handleChordMeta(ctx, *chord, sheetEventRenderer, eventServer);
						continue;
					}
					fm::Ticks ticksToWrite = chord->duration;
					while(ticksToWrite > 1.0_N128) {
						const Event &degree = *(eventServer.nextEvent());
						Event copy; 
						copy = __degreeToAbsoluteNote(ctx, *chord, degree, copy);

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
			for(auto const &templateAndChords : templatesAndItsChords) {
				if (templateAndChords.chords.empty()) {
					continue;
				}
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
							eventServer.onLoop = [this]() {
								// clear mods: https://github.com/SambaGodschynski/werckmeister/issues/99
								ctx_->voiceMetaData()->modifications.clear();
							};
							setTargetCreateIfNotExists(*track, voice);
							ctx_->voiceMetaData()->position = templateAndChords.offset;
							ctx_->voiceMetaData()->tempoFactor = templateAndChords.tempoFactor;
							ctx_->voiceMetaData()->barPosition = 0;

							DEBUGX(
								auto trackname = getMetaValuesBy("name", track->trackInfos).front();
								auto position = ctx_->voiceMetaData()->position;
								auto tempofac = ctx_->voiceMetaData()->tempoFactor;
								std::cout << trackname << " ; " << position << " ; " << tempofac << std::endl;
							)


							Chords chordsPerBar;
							for (const auto &chord : templateAndChords.chords)
							{
								if (chord->type == Event::EOB) {
									__renderOneBar(ctx_, sheetEventRenderer, eventServer, chordsPerBar);
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