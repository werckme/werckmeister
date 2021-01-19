#include "Compiler.h"
#include "sheet/Document.h"
#include <sheet/objects/Event.h>
#include <type_traits>
#include <algorithm>
#include <list>
#include "metaCommands.h"
#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <fm/tools.h>
#include <functional>

namespace sheet {

	namespace compiler {
		void Compiler::compile(DocumentPtr document)
		{
			this->compilerVisitorPtr_->beginCompile();
			this->document_ = document;
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
			logger_->babble(WMLogLambda(log << "preprocess '" << document->path << "'"));
			try {
				preprocessor_->preprocess(document);
				renderChordTrack();
				renderTracks();
			} catch (const fm::Exception &ex) {			
				ex << ex_sheet_document(document);
				throw;
			} catch(...) {
				throw;
			}
			this->compilerVisitorPtr_->endCompile();
		}

		void Compiler::renderTracks()
		{
			auto ctx = context();
			auto renderer = sheetEventRenderer();
			auto document = document_.lock();
			for (auto &track : document->sheetDef.tracks)
			{
				fm::String type = fm::getFirstMetaArgumentWithKeyName(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
				bool isNoteEventTrack = type.empty();
				if (!isNoteEventTrack) {
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
			void consumeChords(Track* sheetTrack, ASheetEventRenderer *sheetEventRenderer, IContext *ctx) 
			{
				using namespace fm;
				auto voice = sheetTrack->voices.begin(); 
				std::list<Event*> barEvents;
				ctx->setChordTrackTarget();
				auto voiceMetaData = ctx->voiceMetaData();
				// since we process the chord track a second time later, we reset all vital values
				voiceMetaData->barPosition = 0;
				voiceMetaData->position =  0;
				voiceMetaData->tempoFactor = 1;
				voiceMetaData->signatureNumerator = 4;
				voiceMetaData->signatureDenominator = 4;
				for (const auto &ev : voice->events) {
					sheetEventRenderer->addEvent(ev);
				}
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
			auto document = document_.lock();
			Track * sheetTrack = getFirstSheetTrack(document->sheetDef.tracks);
			if (!sheetTrack || sheetTrack->voices.empty()) {
				return;
			}
			consumeChords(sheetTrack, sheetEventRenderer().get(), ctx.get());
			sheetTemplateRenderer_->render(sheetTrack);
		}

		ASheetEventRendererPtr Compiler::sheetEventRenderer()
		{
			return this->sheetEventRenderer_;
		}
	}
}