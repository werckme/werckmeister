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
				renderer->__renderEvent__(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::Degree>(SheetEventRenderer* renderer, const Event *ev)
			{
				return true;
			}

			template<>
			bool renderEvent<Event::TiedDegree>(SheetEventRenderer* renderer, const Event *ev)
			{
				return true;
			}

			template<>
			bool renderEvent<Event::TiedNote>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->__renderEvent__(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::PitchBend>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->__renderPitchBendEvent__(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::EOB>(SheetEventRenderer* renderer, const Event *ev)
			{
                auto ctx = renderer->context();
				ctx->newBar(*ev);
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

			template<>
			bool renderEvent<Event::Group>(SheetEventRenderer* renderer, const Event *ev)
			{
				const fm::Ticks eventDuration = ev->duration;
				if (eventDuration == 0) {
					return true;
				}
				fm::Ticks totalGroupEventDuration = 0;
				for (const auto &groupedEvent : ev->eventGroup) {
					totalGroupEventDuration += groupedEvent.duration;
				}
				if (totalGroupEventDuration == 0) {
					return true;
				}
				const fm::Ticks div = totalGroupEventDuration / eventDuration;
				for (const auto &groupedEvent : ev->eventGroup) {
					Event copy = groupedEvent;
					copy.duration /= div;
					renderer->addEvent(copy);
				}
				return true;
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

		void SheetEventRenderer::__renderEvent__(const Event &_ev)
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
			AModification::Events events = { ev };
			auto sanweis = ctx_->spielanweisung();
			if (sanweis) {
				sanweis->perform(ctx_, events);
			}			
			for (auto mod : meta->modifications) {
				mod->perform(ctx_, events);
			}
			for (auto mod : meta->modificationsOnce) {
				mod->perform(ctx_, events);
			}
			auto instrument = ctx_->currentInstrumentDef();
			if (instrument && !instrument->modifications.empty()) {
				for (auto mod : instrument->modifications) {
					mod->perform(ctx_, events);
				}						
			}
			meta->modificationsOnce.clear();
			meta->expression = tmpExpression;
			for (const auto &event : events) {
				if (event.isPitchBend()) {
					__renderPitchBendEvent__(event);
				} else {
					__renderEventPitches__(event);
				}
			}
			ctx_->seek(ev.duration);
		}

		void SheetEventRenderer::__renderEventPitches__(const Event &ev)
		{
			auto ctx = context();
			ctx->seek(ev.offset);
			const auto& pitches = ev.pitches;
			auto duration = ev.duration;
			auto tying = ev.isTied(); 
            auto meta = ctx->voiceMetaData();
			for (const auto &pitch : pitches)
			{
				ctx->renderPitch(pitch, duration, ev.velocity, tying);
			}
			ctx->seek(-ev.offset);
		}

		void SheetEventRenderer::__renderPitchBendEvent__(const Event &pitchBendEvent)
		{
			auto ctx = context();
            auto meta = ctx->voiceMetaData();
			auto position = meta->position + static_cast<fm::Ticks>(pitchBendEvent.offset);
			ctx->renderPitchbend(pitchBendEvent.pitchBendValue, position);
		}
    }
}