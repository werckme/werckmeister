#include "compiler.h"
#include "sheet/Document.h"
#include "AContext.h"
#include "sheet/Event.h"
#include <type_traits>
#include <algorithm>
#include <list>

namespace sheet {

	namespace compiler {

		Compiler::Compiler() 
		{
		}
		void Compiler::compile(DocumentPtr document)
		{
			this->document_ = document;
			auto ctx = context();
			renderTracks();
			renderChordTrack();
		}

		void Compiler::renderTracks()
		{
			auto ctx = context();
			for (const auto &track : document_->sheetDef.tracks)
			{
				auto trackId = ctx->createTrack();
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
				auto it = begin;
				std::list<Event*> barEvents;
				while (it != end) {
					if (it->type == Event::EOB) {
						// TODO: we assume 4/4
						int c = barEvents.size();
						if (c > 0) {
							std::for_each(barEvents.begin(), barEvents.end(), [c](Event *ev) { ev->duration = 4.0_N4 / c; });
						}
						barEvents.clear();
					}
					if (it->type == Event::Chord) {
						barEvents.push_back(&(*it));
					}
					++it;
				}
			}
		}


		void Compiler::renderChordTrack() 
		{
			auto ctx = context();
			determineChordLengths(document_->sheetDef.chords.begin(), document_->sheetDef.chords.end());

			for (const auto &ev : document_->sheetDef.chords) {
				ctx->setChordTrackTarget(); // target will be lost after calling addEvent
				ctx->addEvent(ev);
				if (ev.type == Event::Chord) {
					ctx->renderStyle(ev.duration);
				}
			}
		}
	}
}