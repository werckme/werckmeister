#include "compiler.h"
#include "sheet/Document.h"
#include "sheet/tools.h"
#include "AContext.h"
#include "sheet/Event.h"
#include <type_traits>
#include <algorithm>
#include <list>
#include "sheetTemplateRenderer.h"
#include "metaCommands.h"
#include "error.hpp"
#include <sheet/tools.h>
#include <boost/exception/get_error_info.hpp>
#include <sheet/tools.h>
#include <functional>
#include "preprocessor.h"
#include "sheetEventRenderer.h"

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
				ctx->processMeta(document->sheetDef.sheetInfos, 
					[](const auto &x) { return x.name; }, 
					[](const auto &x) { return x.args; }
				);
			} catch (fm::Exception &ex) {
				if (int *objectIdx = boost::get_error_info<ex_at_object_idx>(ex)) {
					// exception has index on which object the exception occured
					ex << ex_sheet_source_info(document->sheetDef.sheetInfos[*objectIdx]);
				}
				ex << ex_sheet_document(document);
				throw;
			}

			try {
				ctx->metaEventHandler = std::bind(&Compiler::metaEventHandler, this, std::placeholders::_1);
				renderChordTrack();
				renderTracks();
				ctx->metaEventHandler = AContext::MetaEventHandler();
			} catch (fm::Exception &ex) {
				ex << ex_sheet_document(document);
				throw;
			} catch(...) {
				throw;
			}
		}

		bool Compiler::metaEventHandler(const Event &metaEvent)
		{
			if (metaEvent.stringValue == SHEET_META__SHEET_TEMPLATE_POSITION) {
				sheetTemplatePosition(getArgument<fm::String>(metaEvent.metaArgs, 0));
				return true;
			}
			return false;
		}

		void Compiler::sheetTemplatePosition(const fm::String &cmd)
		{
			if (cmd != SHEET_META__SHEET_TEMPLATE_POSITION_CMD) {
				FM_THROW(Exception, "unsupported sheetTemplatePosition command: " + cmd);
			}
			auto ctx = context();
			if (!ctx->capabilities.canSeek) {
				FM_THROW(Exception, "track type does not support sheetTemplatePosition");
			}
			if (currentSheetTemplateRenderer_ == nullptr) {
				FM_THROW(fm::Exception, "sheetTemplate renderer = null");
			}
			FM_THROW(fm::Exception, fm::String(SHEET_META__SHEET_TEMPLATE_POSITION_CMD) + " not implemented");
		}

		void Compiler::renderTracks()
		{
			auto ctx = context();
			ctx->capabilities.canSeek = false;
			Preprocessor preprocessor;
			auto renderer = sheetEventRenderer();
			for (auto &track : document_->sheetDef.tracks)
			{
				fm::String type = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_TYPE, track.trackInfos);
				if (!type.empty()) { // do not render tracks with a specific type
					continue;
				}				
				auto trackId = ctx->createTrack();
				ctx->setTrack(trackId);
				ctx->processMeta(track.trackInfos, 
					[](const auto &x) { return x.name; }, 
					[](const auto &x) { return x.args; }
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
			void preprocessSheetTrack(Track* sheetTrack, AContext *ctx) {
				using namespace fm;
				auto voice = sheetTrack->voices.begin(); 
				auto it = voice->events.begin();
				auto end = voice->events.end();
				std::list<Event*> barEvents;
				ctx->setChordTrackTarget();
				while (it != end) {
					if (it->type == Event::Meta) {
						ctx->setMeta(*it);
					}
					if (it->type == Event::EOB) {
						int nbOfEventsPerBar = barEvents.size();
						if (nbOfEventsPerBar > 0) {
							std::for_each(barEvents.begin(), barEvents.end(), [nbOfEventsPerBar, ctx](Event *ev) { 
								ev->duration = ctx->voiceMetaData()->barLength / nbOfEventsPerBar;
							});
						}
						barEvents.clear();
					}
					if (it->isTimeConsuming()) {
						barEvents.push_back(&(*it));
					}
					++it;
				}
			}
			template<class TContainer>
			Track * getFirstSheetTrack(TContainer &c) {
				auto sheetTrackIt = 
					std::find_if(c.begin(), c.end(), [](const auto &x) {  
						return getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_TYPE, x.trackInfos) == SHEET_META__TRACK_META_VALUE_TYPE_SHEET;
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
			ctx->capabilities.canSeek = true;
			Track * sheetTrack = getFirstSheetTrack(document_->sheetDef.tracks);
			if (!sheetTrack || sheetTrack->voices.empty()) {
				return;
			}
			preprocessSheetTrack(sheetTrack, ctx.get());
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