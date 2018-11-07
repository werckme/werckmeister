#include "compiler.h"
#include "sheet/Document.h"
#include "AContext.h"
#include "sheet/Event.h"
#include <type_traits>

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
			renderTracks();
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
	}
}