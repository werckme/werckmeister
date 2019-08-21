#include "sheetEventRenderer.h"
#include "spielanweisung/ASpielanweisung.h"
#include "spielanweisung/spielanweisungen.h"
#include "spielanweisung/Vorschlag.h"
#include "modification/AModification.h"

namespace sheet {
    namespace compiler {
        namespace {
			template<int EventType>
			bool renderEvent(SheetEventRenderer* renderer, const Event *ev)
			{
				return false;
			}

			template<>
			bool renderEvent<Event::Note>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->renderEvent(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::Degree>(SheetEventRenderer* renderer, const Event *ev)
			{
                 auto ctx = renderer->context();
				auto chordDef = ctx->currentChordDef();
				auto chord = ctx->currentChord();
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(*chord, *chordDef, ev->pitches, ctx->getTimeInfo());
				Event copy = *ev;
				copy.type = Event::Note;
				copy.pitches.swap(pitches);
				renderer->renderEvent(copy);
				return true;
			}

			template<>
			bool renderEvent<Event::TiedDegree>(SheetEventRenderer* renderer, const Event *ev)
			{
                auto ctx = renderer->context();
				auto chordDef = ctx->currentChordDef();
				auto chord = ctx->currentChord();
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(*chord, *chordDef, ev->pitches, ctx->getTimeInfo());
				Event copy = *ev;
				copy.type = Event::TiedNote;
				copy.pitches.swap(pitches);
				renderer->renderEvent(copy);

				return true;
			}

			template<>
			bool renderEvent<Event::TiedNote>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->renderEvent(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::EOB>(SheetEventRenderer* renderer, const Event *ev)
			{
                auto ctx = renderer->context();
				ctx->newBar();
				return true;
			}

			template<>
			bool renderEvent<Event::Rest>(SheetEventRenderer* renderer, const Event *ev)
			{
                auto ctx = renderer->context();
				ctx->rest(ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::Chord>(SheetEventRenderer* renderer, const Event *chordEv)
			{
                auto ctx = renderer->context();
				ctx->setChord(*chordEv);
				ctx->seek(chordEv->duration);
				return true;
			}
			template<>
			bool renderEvent<Event::Meta>(SheetEventRenderer* renderer, const Event *ev)
			{
                auto ctx = renderer->context();
				ctx->setMeta(*ev);
				return true;
			}
			//////////////////////////////////////////////////
			template <int EventId>
			bool renderEventUnrolled(SheetEventRenderer* renderer, const Event *ev)
			{
				if (ev->type == EventId) {
					return renderEvent<EventId>(renderer, ev);
				}
				return renderEventUnrolled<EventId + 1>(renderer, ev);
			}
			template <>
			bool renderEventUnrolled<Event::NumEvents>(SheetEventRenderer* renderer, const Event *ev)
			{
				return false;
			}

			void _addEvent(SheetEventRenderer* renderer, const Event *ev)
			{
				renderEventUnrolled<0>(renderer, ev);
			}
		}

        void SheetEventRenderer::addEvent(const Event &ev)
		{
			auto meta = ctx_->voiceMetaData();
			++(meta->eventCount);
			try {
				_addEvent(this, &ev);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_source_info(ev);
				throw;
			}
		}

		void SheetEventRenderer::renderEvent(const Event &_ev)
		{
			Event ev = _ev;
			auto meta = ctx_->voiceMetaData();
			auto tmpExpression = meta->expression;

			if (meta->singleExpression != fm::expression::Default) {
				tmpExpression = meta->expression;
				meta->expression = meta->singleExpression;
				meta->singleExpression = fm::expression::Default;
			}
			ev.velocity = ctx_->velocity();
			for (auto mod : meta->modifications) {
				mod->perform(ctx_, ev);
			}
			for (auto mod : meta->modificationsOnce) {
				mod->perform(ctx_, ev);
			}			
			meta->modificationsOnce.clear();
			auto sanweis = ctx_->spielanweisung();
			sanweis->perform(ctx_, ev);
			meta->expression = tmpExpression;
		}
    }
}