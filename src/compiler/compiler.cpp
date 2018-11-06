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
				static_assert(false, "Invalid Event Type");
			}

			template<>
			bool renderEvent<Event::Note>(AContextPtr ctx, const Event &ev)
			{
				for (const auto &pitch : ev.pitches)
				{
					ctx->addEvent(pitch, ev.duration);
				}
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
			bool renderEvent(AContextPtr ctx, const Event &ev)
			{
				// TODO: replace using variadic templates eg. handleEvent<Event::Note, Event::EOB, Event::Rest>(ctx, ev)
				switch (ev.type)
				{
				case Event::Note:
					return renderEvent<Event::Note>(ctx, ev);
				case Event::EOB:
					return renderEvent<Event::EOB>(ctx, ev);
				case Event::Rest:
					return renderEvent<Event::Rest>(ctx, ev);
				default:
					return false;
				}
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