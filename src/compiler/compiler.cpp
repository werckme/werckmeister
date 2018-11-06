#include "compiler.h"
#include "sheet/Document.h"
#include "AContext.h"
#include "sheet/Event.h"


namespace sheet {
	namespace compiler {

		namespace {
			template<int EventType>
			bool renderEvent(AContextPtr ctx, const Event &ev)
			{
				for (const auto &pitch : ev.pitches)
				{
					ctx->addEvent(pitch, ev.duration);
				}
				return true;
			}
			bool renderEvent(AContextPtr ctx, const Event &ev)
			{
				switch (ev.type)
				{
				case Event::Note:
					return renderEvent<Event::Note>(ctx, ev);
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