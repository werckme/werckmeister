#include "Compiler.h"
#include "sheet/Document.h"
#include "context/AContext.h"
#include <sheet/objects/Event.h>
#include <type_traits>
#include <algorithm>
#include <list>
#include "SheetTemplateRenderer.h"
#include "metaCommands.h"
#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <fm/tools.h>
#include <functional>
#include "Preprocessor.h"
#include "SheetEventRenderer.h"

namespace sheet {

	namespace compiler {

		Compiler::Compiler() 
		{
		}
		Compiler::~Compiler() 
		{
		}
		void Compiler::compile(DocumentPtr document)
		{
			this->document_ = document;
			auto ctx = context();

			try {
				sheetEventRenderer()->handleMetaEvents(document->sheetDef.documentConfigs, 
					[](const auto &x) { 
						sheet::Event metaEvent;
						metaEvent.type = sheet::Event::Meta;
						metaEvent.stringValue = x.name;
						metaEvent.metaArgs = x.args;
						return metaEvent;
					 }
				);
			} catch (fm::Exception &ex) {
				ex << ex_sheet_document(document);
				throw;
			}

			try {
				renderChordTrack();
				renderTracks();
			} catch (fm::Exception &ex) {			
				ex << ex_sheet_document(document);
				throw;
			} catch(...) {
				throw;
			}
		}

		void Compiler::renderTracks()
		{
			auto ctx = context();
			Preprocessor preprocessor;
			auto renderer = sheetEventRenderer();
			for (auto &track : document_->sheetDef.tracks)
			{
				fm::String type = fm::getFirstMetaArgumentWithKeyName(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
				if (!type.empty()) { // do not render tracks with a specific type
					continue;
				}				
				auto trackId = ctx->createTrack();
				ctx->setTrack(trackId);
				renderer->handleMetaEvents(track.trackConfigs, 
					[](const auto &x) { 
						sheet::Event metaEvent;
						metaEvent.type = sheet::Event::Meta;
						metaEvent.stringValue = x.name;
						metaEvent.metaArgs = x.args;
						return metaEvent;
					}
				);
				preprocessor.process(track);
				for (const auto &voice : track.voices)
				{
					auto voiceId = ctx->createVoice();
					ctx->setTarget(trackId, voiceId);
					for (const auto &ev : voice.events)
					{
						renderer->addEvent(ev);
					}
				}
			}
		}

		namespace {
			void consumeChords(Track* sheetTrack, SheetEventRenderer *sheetEventRenderer, AContext *ctx) 
			{
				using namespace fm;
				auto voice = sheetTrack->voices.begin(); 
				std::list<Event*> barEvents;
				ctx->setChordTrackTarget();
				auto voiceMetaData = ctx->voiceMetaData();
				// since we process the chord track a second time, we reset all vital values
				voiceMetaData->barPosition = 0;
				voiceMetaData->position =  0;
				voiceMetaData->tempoFactor = 1;
				voiceMetaData->signatureNumerator = 4;
				voiceMetaData->signatureDenominator = 4;
				for (const auto &ev : voice->events) {
					sheetEventRenderer->addEvent(ev);
				}
			}
			void preprocessSheetTrack(Track* sheetTrack, SheetEventRenderer *renderer, AContext *ctx) {
				using namespace fm;
				auto voice = sheetTrack->voices.begin(); 
				auto it = voice->events.begin();
				auto end = voice->events.end();
				std::list<Event*> barEvents;
				ctx->setChordTrackTarget();
				auto processEob = [&barEvents, ctx]() {
					int nbOfEventsPerBar = barEvents.size();
					if (nbOfEventsPerBar > 0) {
						std::for_each(barEvents.begin(), barEvents.end(), [nbOfEventsPerBar, ctx](Event *ev) {
							auto barLength = ctx->voiceMetaData()->barLength;
							ev->duration = barLength / nbOfEventsPerBar;
						});
					}
					barEvents.clear();
				};
				while (it != end) {
					if (it->type == Event::Meta) {
						renderer->handleMetaEvent(*it);
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
			template<class TContainer>
			Track * getFirstSheetTrack(TContainer &c) {
				auto sheetTrackIt = 
					std::find_if(c.begin(), c.end(), [](const auto &x) {  
						return fm::getFirstMetaArgumentWithKeyName(SHEET_META__TRACK_META_KEY_TYPE, x.trackConfigs).value == SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP;
					});
				if (sheetTrackIt == c.end()) {
					return nullptr;
				}
				return &(*sheetTrackIt);
			} 
		}


		void Compiler::renderChordTrack() 
		{
			auto ctx = context();
			Track * sheetTrack = getFirstSheetTrack(document_->sheetDef.tracks);
			if (!sheetTrack || sheetTrack->voices.empty()) {
				return;
			}
			preprocessSheetTrack(sheetTrack, sheetEventRenderer().get(), ctx.get());
			consumeChords(sheetTrack, sheetEventRenderer().get(), ctx.get());
			SheetTemplateRenderer sheetTemplateRenderer(ctx.get(), sheetEventRenderer().get());
			sheetTemplateRenderer.render(sheetTrack);
		}

		SheetEventRendererPtr Compiler::sheetEventRenderer()
		{
			if (!this->sheetEventRenderer_) {
				this->sheetEventRenderer_ = std::make_shared<SheetEventRenderer>(context().get());
			}
			return this->sheetEventRenderer_;
		}
		void Compiler::sheetEventRenderer(SheetEventRendererPtr renderer)
		{
			this->sheetEventRenderer_ = renderer;
		}
	}
}