#include "compiler.h"
#include "sheet/Document.h"
#include "sheet/tools.h"
#include "AContext.h"
#include "sheet/Event.h"
#include <type_traits>
#include <algorithm>
#include <list>
#include "styleRenderer.h"
#include "metaCommands.h"
#include "error.hpp"
namespace sheet {

	namespace compiler {

		Compiler::Compiler() 
		{
		}
		void Compiler::compile(DocumentPtr document)
		{
			this->document_ = document;
			auto ctx = context();
			ctx->processMeta(document->sheetDef.sheetInfos, 
				[](const auto &x) { return x.name; }, 
				[](const auto &x) { return x.args; }
			);
			renderChordTrack();
			renderTracks();
		}

		void Compiler::renderTracks()
		{
			auto ctx = context();
			for (const auto &track : document_->sheetDef.tracks)
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
				
				for (const auto &voice : track.voices)
				{
					auto voiceId = ctx->createVoice();
					ctx->setTarget(trackId, voiceId);
					for (const auto &ev : voice.events)
					{
						ctx->addEvent(ev);
					}
				}
			}
		}

		namespace {
			template<typename TIt>
			void determineChordLengths(TIt begin, TIt end) {
				using namespace fm;
				typedef Event::Multiplicator Multiplicator;
				auto it = begin;
				std::list<Event*> barEvents;
				while (it != end) {
					if (it->type == Event::EOB) {
						int c = barEvents.size();
						if (c > 0) {
							std::for_each(barEvents.begin(), barEvents.end(), [c](Event *ev) { 
								ev->multiplicator = 1.0 / (Multiplicator)c; 
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
		}

		void Compiler::switchStyle(StyleRenderer &styleRenderer, const Event &metaEvent)
		{
			auto file = getArgument<fm::String>(metaEvent, 0);
			auto part = getArgument<fm::String>(metaEvent, 1);
			auto ctx = styleRenderer.context();
			auto style = ctx->styleDefServer()->getStyle(file, part);
			if (style.empty()) {
				FM_THROW(Exception, "style not found: " + fm::to_string(file) + " " + fm::to_string(part));
			}
			styleRenderer.switchStyle(ctx->currentStyle(), style);
		}

		void Compiler::renderChordTrack() 
		{
			// auto ctx = context();
			// determineChordLengths(document_->sheetDef.chords.begin(), document_->sheetDef.chords.end());
			// StyleRenderer styleRenderer(ctx);
			// for (auto &ev : document_->sheetDef.chords) {
			// 	ctx->setChordTrackTarget(); // target will be lost after calling addEvent
			// 	if (ev.type == Event::Rest) {
			// 		auto meta = ctx->voiceMetaData(ctx->chordVoiceId());
			// 		ev.duration = meta->barLength * ev.multiplicator;
			// 		ctx->rest(ev.duration);
			// 		styleRenderer.sheetRest(ev.duration);
			// 	}
			// 	else if (ev.stringValue == SHEET_META__SET_STYLE) {
			// 		switchStyle(styleRenderer, ev);
			// 	}
			// 	else if (ev.type != Event::Chord) {
			// 		ctx->addEvent(ev);
			// 	} else {
			// 		auto meta = ctx->voiceMetaData(ctx->chordVoiceId());
			// 		ev.duration = meta->barLength * ev.multiplicator;	
			// 		ctx->addEvent(ev);
			// 		styleRenderer.render(ev.duration);
			// 	}
			// }
		}
	}
}