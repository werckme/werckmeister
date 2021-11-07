#include "SheetTemplateRenderer.h"
#include "error.hpp"
#include <fm/tools.h>
#include <iostream>
#include <boost/exception/get_error_info.hpp>
#include "SheetEventRenderer.h"
#include <algorithm>
#include <functional>
#include <fm/werckmeister.hpp>
#include <fm/IDefinitionsServer.h>
#include <compiler/commands/Fill.h>

#define DEBUGX(x)

namespace sheet {
	namespace compiler {

		namespace {
			struct TemplatesAndItsChords {
				typedef std::vector<Event*> Chords;
				typedef IContext::SheetTemplates Templates;
				Chords chords;
				Templates templates;
				fm::Ticks offset = 0;
				double tempoFactor = 1;
			};
			class DegreeEventServer {
				const Voice::Events* degrees_;
				Voice::Events::const_iterator it_;
				void degrees(const Voice::Events* degrees);
			public:
				bool templateIsFill = false;
				fm::Ticks ignoreUntilPosition = -1.0;
				typedef std::function<void()> OnLoop;
				OnLoop onLoop;
				DegreeEventServer(const Voice::Events* degrees);
				std::pair<const Event*, fm::Ticks> nextEvent(IContextPtr ctx);
				void seek(fm::Ticks);
				bool hasFurtherEvents() const
				{
					return this->it_ != degrees_->end();
				}

			};
			DegreeEventServer::DegreeEventServer(const Voice::Events* degrees)
			{
				this->degrees(degrees);
			}
			void DegreeEventServer::degrees(const Voice::Events* degrees)
			{
				this->degrees_ = degrees;
				it_ = this->degrees_->begin();
			}
			std::pair<const Event*, fm::Ticks> DegreeEventServer::nextEvent(IContextPtr ctx)
			{
				if (it_ == degrees_->end()) {
					if (this->templateIsFill) {
						return std::make_pair(nullptr, 0.0);
					}
					seek(0);
				}
				const auto* ev = &(*it_++);
				if (!ev->isTimeConsuming()) {
					return std::make_pair(ev, ev->duration);
				}
				auto position = ctx->currentPosition();
				if (position < ignoreUntilPosition) {
					return std::make_pair(nullptr, ev->duration);
				}
				return std::make_pair(ev, ev->duration);
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

			sheet::SheetTemplate __getTemplate(SheetTemplateRenderer& sheetTemplateRenderer, const fm::String& sheetTemplateName)
			{
				auto ctx = sheetTemplateRenderer.context();
				auto sheetTemplate = ctx->definitionsServer()->getSheetTemplate(sheetTemplateName);
				if (sheetTemplate.empty()) {
					FM_THROW(Exception, "sheetTemplate not found: " + sheetTemplateName);
				}
				return sheetTemplate;
			}

			IContext::SheetTemplates __getTemplates(SheetTemplateRenderer& sheetTemplateRenderer, const Event& metaEvent)
			{
				auto ctx = sheetTemplateRenderer.context();
				IContext::SheetTemplates templates;
				for (size_t idx = 0; idx < metaEvent.metaArgs.size(); ++idx) {
					auto sheetTemplateName = fm::getArgumentValue<fm::String>(metaEvent, idx);
					auto sheetTemplate = __getTemplate(sheetTemplateRenderer, sheetTemplateName);
					templates.push_back(sheetTemplate);
				}
				return templates;
			}

			template<class TCommand>
			std::shared_ptr<TCommand> __getCommand(const Event& ev)
			{
				auto& wm = fm::getWerckmeister();
				auto fillCommand = wm.solveOrDefault<TCommand>(ev.stringValue);
				if (!fillCommand) {
					FM_THROW(Exception, "expecting command");
				}
				fillCommand->setArguments(ev.metaArgs);
				return fillCommand;
			}

			std::list<TemplatesAndItsChords> __collectChordsPerTemplate(SheetTemplateRenderer& sheetTemplateRenderer, Track* sheetTrack)
			{
				auto ctx = sheetTemplateRenderer.context();
				std::list<TemplatesAndItsChords> templatesAndItsChords;
				templatesAndItsChords.emplace_back(TemplatesAndItsChords());
				templatesAndItsChords.back().templates = ctx->currentSheetTemplates();
				auto& wm = fm::getWerckmeister();
				auto tmpContext = ctx->createNewContext();
				tmpContext->masterTempo(ctx->masterTempo());
				tmpContext->setChordTrackTarget();
				auto tmpEventRenderer = sheetTemplateRenderer.sheetEventRenderer->createNewSheetEventRenderer(tmpContext);

				auto& sheetEvents = sheetTrack->voices.begin()->events;
				for (auto& ev : sheetEvents) {
					try {
						bool isTempoEvent = ev.stringValue == SHEET_META__SET_TEMPO;
						bool isFillTemplate = ev.stringValue == SHEET_META__SET_FILL_TEMPLATE;
						bool isTemplateEvent = ev.stringValue == SHEET_META__SET_SHEET_TEMPLATE;
						if (isTemplateEvent) {
							TemplatesAndItsChords::Templates prevTemplates;							
							fm::Ticks prevOffset = -1;
							if (!templatesAndItsChords.empty()) {
								prevTemplates = templatesAndItsChords.back().templates;
								prevOffset = templatesAndItsChords.back().offset;
							}
							// new template
							templatesAndItsChords.emplace_back(TemplatesAndItsChords());
							auto& newTemplateAndChords = templatesAndItsChords.back();
							newTemplateAndChords.templates = __getTemplates(sheetTemplateRenderer, ev);
							newTemplateAndChords.offset = tmpContext->currentPosition();
							newTemplateAndChords.tempoFactor = tmpContext->voiceMetaData()->tempoFactor;
							for (auto& prevTemplate : prevTemplates) {
								// in case template comes after fill, readd fills
								if (!prevTemplate.isFill) {
									continue;
								}
								if (prevOffset >= 0 && prevOffset != newTemplateAndChords.offset) {
									continue;
								}
								newTemplateAndChords.templates.push_back(prevTemplate);
							}
						}
						else if (isFillTemplate) {
							auto fillCommand = __getCommand<Fill>(ev);
							TemplatesAndItsChords::Templates prevTemplates;
							fm::Ticks prevOffset = -1;
							if (!templatesAndItsChords.empty()) {
								prevTemplates = templatesAndItsChords.back().templates;
								prevOffset = templatesAndItsChords.back().offset;
							}
							templatesAndItsChords.emplace_back(TemplatesAndItsChords());
							auto& newTemplateAndChords = templatesAndItsChords.back();
							auto fillTemplate = __getTemplate(sheetTemplateRenderer, fillCommand->templateName());
							fillTemplate.isFill = true;
							newTemplateAndChords.templates = { fillTemplate };
							newTemplateAndChords.offset = tmpContext->currentPosition();
							newTemplateAndChords.tempoFactor = tmpContext->voiceMetaData()->tempoFactor;
							fm::String replaceTemplateName = fillCommand->replaceTemplateName();
							for (auto& prevTemplate : prevTemplates) {
								// add running templates
								if (prevTemplate.isFill && (prevOffset >= 0 && prevOffset != newTemplateAndChords.offset)) {
									continue;
								}
								bool isReplacedByFill = prevTemplate.name == fillCommand->replaceTemplateName();
								if (isReplacedByFill) {
									prevTemplate.ignoreUnitlPosition = tmpContext->currentPosition() + fillTemplate.maxLength();
								}
								newTemplateAndChords.templates.push_back(prevTemplate);
							}
						}
						else {
							// add any other event
							auto& currentTemplateAndChords = templatesAndItsChords.back();
							currentTemplateAndChords.chords.push_back(&ev);
							if (ev.type == Event::Meta) {
								tmpEventRenderer->handleMetaEvent(ev);
								if (isTempoEvent) {
									templatesAndItsChords.emplace_back(TemplatesAndItsChords());
									auto& newTemplateAndChords = templatesAndItsChords.back();
									newTemplateAndChords.templates = currentTemplateAndChords.templates;
									newTemplateAndChords.tempoFactor = tmpContext->voiceMetaData()->tempoFactor;
									newTemplateAndChords.offset = tmpContext->currentPosition();
								}
							}
							if (ev.isTimeConsuming()) {
								tmpContext->seek(ev.duration);
							}
						}
					}
					catch (const Exception& ex) {
						ex << ex_sheet_source_info(ev);
						throw;
					}
				}
				return templatesAndItsChords;
			}

			Event& __degreeToAbsoluteNote(IContextPtr ctx, const Event& chordEvent, const Event& degreeEvent, Event& target) {
				try {
					target = degreeEvent;
					if (degreeEvent.type == Event::Group) {
						target.type = Event::Group;
						size_t index = 0;
						for (const auto& groupedDegreeEvent : degreeEvent.eventGroup) {
							__degreeToAbsoluteNote(ctx, chordEvent, groupedDegreeEvent, target.eventGroup[index++]);
						}
						return target;
					}
					if (!degreeEvent.isRelativeDegree()) {
						return target;
					}
					auto chordDef = ctx->definitionsServer()->getChord(chordEvent.chordDefName());
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
				catch (const Exception& ex) {
					ex << ex_sheet_source_info(chordEvent);
					throw;
				}
			}

			void __handleTemplatePositionCmd(const Event& metaEvent, DegreeEventServer& eventServer)
			{
				try {
					const auto& args = metaEvent.metaArgs;
					if (args.empty()) {
						FM_THROW(Exception, fm::String("no args for: ") + SHEET_META__SHEET_TEMPLATE_POSITION);
					}
					auto arg = args.front();
					if (arg.value == SHEET_META__SHEET_TEMPLATE_POSITION_CMD_RESET) {
						eventServer.seek(0);
					}
					else {
						FM_THROW(Exception, fm::String("invalid arg for: ") + SHEET_META__SHEET_TEMPLATE_POSITION + ": " + arg.value);
					}
				}
				catch (fm::Exception& ex) {
					ex << ex_sheet_source_info(metaEvent);
					throw;
				}
			}

			void __handleChordMeta(IContextPtr ctx,
				const Event& metaEvent,
				ASheetEventRenderer* sheetEventRenderer,
				DegreeEventServer& eventServer)
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
			fm::Ticks __renderOneBar(IContextPtr ctx,
				ASheetEventRenderer* sheetEventRenderer,
				DegreeEventServer& eventServer,
				const Chords& chords,
				fm::String& out_lastChord)
			{
				fm::Ticks leftover = 0;
				fm::Ticks written = 0;
				int numberOfChords = std::count_if(chords.begin(), chords.end(), [](auto& ev) {
					return ev->type == Event::Chord;
					});
				for (const Event* chord : chords) {
					using namespace fm;
					if (chord->type == Event::Rest && numberOfChords == 0) {
						// if the bar just contains this one rest we can skip the whole thing
						ctx->seek(chord->duration);
						continue;
					}
					if (chord->type == Event::Meta) {
						__handleChordMeta(ctx, *chord, sheetEventRenderer, eventServer);
						continue;
					}
					if (chord->type == Event::Meta) {
						__handleChordMeta(ctx, *chord, sheetEventRenderer, eventServer);
						continue;
					}
					if (chord->type == Event::Chord && chord->stringValue != out_lastChord) {
						// chord changed, we can't be sure that all of our tied notes will be stopped
						// so we stop them now
						out_lastChord = chord->stringValue;
						ctx->stopAllPendingTies();
					}
					fm::Ticks ticksToWrite = chord->duration;
					while (ticksToWrite > 1.0_N128) {
						const Event* degree = nullptr;
						fm::Ticks degreeDuration;
						std::tie(degree, degreeDuration) = eventServer.nextEvent(ctx);
						if (degree == nullptr && degreeDuration == 0) {
							// no more events to come
							ctx->rest(ticksToWrite);
							break;
						}
						Event copy;
						if (degree == nullptr) {
							copy.duration = degreeDuration;
							copy.type = Event::Rest;
						}
						else {
							if (chord->type == Event::Rest) {
								copy = *degree;
								copy.type = Event::Rest;
							}
							else {
								copy = __degreeToAbsoluteNote(ctx, *chord, *degree, copy);
							}
						}
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


		SheetTemplateRenderer::SheetTemplateRenderer(IContextPtr  ctx, ASheetEventRendererPtr renderer) :
			sheetEventRenderer(renderer)
			, ctx_(ctx)
		{
		}

		SheetTemplateRenderer::~SheetTemplateRenderer()
		{
		}

		SheetTemplateRenderer::ContainerKeyType SheetTemplateRenderer::getKey(const Track& track) const
		{
			auto instrumentName = fm::getFirstMetaArgumentForKey(SHEET_META__INSTRUMENT, track.trackConfigs);
			if (instrumentName.value.empty()) {
				return std::to_string((long long)(&track));
			}
			return instrumentName.value;
		}

		SheetTemplateRenderer::ContainerKeyType SheetTemplateRenderer::getKey(const Voice& voice) const
		{
			return std::to_string((long long)(&voice));
		}

		void SheetTemplateRenderer::setTargetCreateIfNotExists(const Track& track, const Voice& voice)
		{
			IContext::TrackId trackId;
			IContext::VoiceId voiceId;
			auto trackKey = getKey(track);
			auto it = _contextElementIdMap.find(trackKey);
			bool trackIsNew = false;
			if (it == _contextElementIdMap.end()) {
				trackId = ctx_->createTrack();
				_contextElementIdMap[trackKey] = trackId;
				trackIsNew = true;
			}
			else {
				trackId = it->second;
			}
			auto voiceKey = getKey(voice);
			it = _contextElementIdMap.find(voiceKey);
			if (it == _contextElementIdMap.end()) {
				voiceId = ctx_->createVoice();
				_contextElementIdMap[voiceKey] = voiceId;
			}
			else {
				voiceId = it->second;
			}
			ctx_->setTarget(trackId, voiceId);
			if (trackIsNew) {
				try {
					sheetEventRenderer->handleMetaEvents(track.trackConfigs,
						[](const auto& x) {
							sheet::Event metaEvent;
							metaEvent.type = sheet::Event::Meta;
							metaEvent.stringValue = x.name;
							metaEvent.metaArgs = x.args;
							return metaEvent;
						}
					);
				}
				catch (fm::Exception& ex) {
					throw;
				}
			}
		}


		void SheetTemplateRenderer::render(Track* sheetTrack)
		{
			auto sheetMeta = ctx_->voiceMetaData(ctx_->chordVoiceId());
			auto templatesAndItsChords = __collectChordsPerTemplate(*this, sheetTrack);
			for (auto const& templateAndChords : templatesAndItsChords) {
				if (templateAndChords.chords.empty()) {
					continue;
				}
				for (const auto& tmpl : templateAndChords.templates) {
					const auto& sheetTemplateTracks = tmpl.tracks;
					for (const auto& track : sheetTemplateTracks) { 
						for (const auto& voice : track->voices) {
							if (voice.events.empty()) {
								continue;
							}
							DegreeEventServer eventServer(&(voice.events));
							eventServer.templateIsFill = tmpl.isFill;
							eventServer.ignoreUntilPosition = tmpl.ignoreUnitlPosition;
							eventServer.onLoop = [this]() {
								// clear mods: https://github.com/SambaGodschynski/werckmeister/issues/99
								ctx_->voiceMetaData()->modifications.clear();
							};
							setTargetCreateIfNotExists(*track, voice);
							ctx_->voiceMetaData()->position = templateAndChords.offset;
							ctx_->voiceMetaData()->tempoFactor = templateAndChords.tempoFactor;
							ctx_->voiceMetaData()->barPosition = 0;

							DEBUGX({
								auto trackname = getMetaArgumentsForKey("name", track->trackConfigs).front();
								auto position = ctx_->voiceMetaData()->position;
								auto tempofac = ctx_->voiceMetaData()->tempoFactor;
								std::cout << trackname << " ; " << position << " ; " << tempofac << std::endl;
							});

							Chords chordsPerBar;
							fm::String lastChord;
							std::function<bool(const Event*)> renderBarOrCollectEvent = [&](const Event* chord) {
								if (chord->type == Event::EOB) {
									const auto* eobEvent = chord;
									__renderOneBar(ctx_, sheetEventRenderer.get(), eventServer, chordsPerBar, lastChord);
									sheetEventRenderer->addEvent(*eobEvent);
									chordsPerBar.clear();
									if (eventServer.templateIsFill && !eventServer.hasFurtherEvents()) {
										return false;
									}
									return true;
								}
								if (chord->type == Event::Chord
									|| chord->type == Event::Meta
									|| chord->type == Event::Rest)
								{
									chordsPerBar.push_back(chord);
								}
								return true;
							};

							for (const auto& chord : templateAndChords.chords)
							{
								bool continue_ = renderBarOrCollectEvent(chord);
								if (!continue_) {
									break;
								}
							}
							if (eventServer.templateIsFill && eventServer.hasFurtherEvents())
							{
								while (eventServer.hasFurtherEvents()) {
									for (const auto& chord : templateAndChords.chords)
									{
										bool continue_ = renderBarOrCollectEvent(chord);
										if (!continue_) {
											break;
										}
									}
									// make sure we render all remainings (#237)
									if (!chordsPerBar.empty()) {
										__renderOneBar(ctx_, sheetEventRenderer.get(), eventServer, chordsPerBar, lastChord);
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