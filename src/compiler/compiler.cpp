#include "compiler.h"
#include "sheet/Document.h"
#include "AContext.h"
#include "sheet/Event.h"
#include <type_traits>
#include <algorithm>
#include <list>

namespace sheet {
	namespace compiler {

		namespace {
			template<int EventType>
			bool renderEvent(AContextPtr ctx, const Event &ev)
			{
				return false;
			}

			template<>
			bool renderEvent<Event::Note>(AContextPtr ctx, const Event &ev)
			{
				for (const auto &pitch : ev.pitches)
				{
					ctx->addEvent(pitch, ev.duration);
				}
				ctx->seek(ev.duration);
				return true;
			}

			template<>
			bool renderEvent<Event::TiedNote>(AContextPtr ctx, const Event &ev)
			{
				for (const auto &pitch : ev.pitches)
				{
					ctx->addEvent(pitch, ev.duration, true);
				}
				ctx->seek(ev.duration);
				return true;
			}

			template<>
			bool renderEvent<Event::EOB>(AContextPtr ctx, const Event &ev)
			{
				ctx->newBar();
				return true;
			}

			template<>
			bool renderEvent<Event::Rest>(AContextPtr ctx, const Event &ev)
			{
				ctx->rest(ev.duration);
				return true;
			}

			template<>
			bool renderEvent<Event::Chord>(AContextPtr ctx, const Event &ev)
			{
				ctx->rest(ev.duration);
				return true;
			}
			//////////////////////////////////////////////////
			template <int EventId>
			bool renderEventUnrolled(AContextPtr ctx, const Event &ev)
			{
				if (ev.type == EventId) {
					return renderEvent<EventId>(ctx, ev);
				}
				return renderEventUnrolled<EventId + 1>(ctx, ev);
			}
			template <>
			bool renderEventUnrolled<Event::NumEvents>(AContextPtr ctx, const Event &ev)
			{
				return false;
			}

			void renderEvent(AContextPtr ctx, const Event &ev)
			{
				renderEventUnrolled<0>(ctx, ev);
			}
		}
	}
}

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
						renderEvent(ctx, ev);
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

		void Compiler::renderStyle(const ChordDef::Intervals *, const StyleDef *style, fm::Ticks duration)
		{
			
		}

		void Compiler::renderChordTrack() 
		{
			auto ctx = context();
			auto trackId = ctx->createTrack();
			auto voiceId = ctx->createVoice();
			determineChordLengths(document_->sheetDef.chords.begin(), document_->sheetDef.chords.end());
			for (const auto &ev : document_->sheetDef.chords) {
				if (ev.type == Event::Chord) {
					const ChordDef::Intervals *def = document_->getChord(ev.chordName);
					if (!def) {
						ctx->throwContextException("chord not found: " + fm::to_string(ev.chordName));
					}
					const StyleDef *style = document_->getStyle(L"");
					renderStyle(def, style, ev.duration);
				}
			}
		}
	}
}