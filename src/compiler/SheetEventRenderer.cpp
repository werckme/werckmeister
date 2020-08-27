#include "SheetEventRenderer.h"
#include "spielanweisung/ASpielanweisung.h"
#include "spielanweisung/spielanweisungen.h"
#include "spielanweisung/Vorschlag.h"
#include "modification/AModification.h"
#include <fm/werckmeister.hpp>
#include <compiler/commands/ACommand.h>
#include <compiler/commands/AUsingAnEvent.h>
#include <compiler/Warning.hpp>
#include "IEventLogger.h"

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
				renderer->renderEvent(*ev);
				return true;
			}

			template<>
			bool renderEvent<Event::PitchBend>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->renderPitchBendEvent(*ev);
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
				ctx->seek(chordEv->duration);
				return true;
			}
			template<>
			bool renderEvent<Event::Meta>(SheetEventRenderer* renderer, const Event *ev)
			{
				renderer->handleMetaEvent(*ev);
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

		std::shared_ptr<ASheetEventRenderer> SheetEventRenderer::createNewSheetEventRenderer(IContextPtr ctx)
		{
			return std::make_shared<SheetEventRenderer>(ctx, compilerVisitor_, logger_);
		}

        void SheetEventRenderer::addEvent(const Event &ev)
		{
			compilerVisitor_->visit(ctx_.get(), ev);
			auto meta = ctx_->voiceMetaData();
			++(meta->eventCount);
			try {
				ctx_->warningHandler(std::bind(&SheetEventRenderer::onWarning, this, std::placeholders::_1, ev));
				_addEvent(this, &ev);
				ctx_->warningHandler(nullptr);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_source_info(ev);
				throw;
			}
		}

		void SheetEventRenderer::onWarning(const fm::String &message, const Event &event)
		{
			IEventLoggerPtr eventLogger = std::dynamic_pointer_cast<IEventLogger>(logger_);
			if (!eventLogger) {
				logger_->warn(WMLogLambda(log << message));
				return;
			}
			eventLogger->warn(WMLogLambda(log << message), event);
		}

		void SheetEventRenderer::renderEvent(const Event &_ev)
		{
			Event ev = _ev;
			auto meta = ctx_->voiceMetaData();
			auto tmpExpression = meta->expression;
			if (meta->expressionPlayedOnce != fm::expression::Default) {
				tmpExpression = meta->expression;
				meta->expression = meta->expressionPlayedOnce;
				meta->expressionPlayedOnce = fm::expression::Default;
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
					renderPitchBendEvent(event);
				} else {
					renderEventPitches(event);
				}
			}
			ctx_->seek(ev.duration);
		}

		void SheetEventRenderer::renderEventPitches(const Event &ev)
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

		void SheetEventRenderer::renderPitchBendEvent(const Event &pitchBendEvent)
		{
			auto ctx = context();
            auto meta = ctx->voiceMetaData();
			auto position = meta->position + static_cast<fm::Ticks>(pitchBendEvent.offset);
			ctx->renderPitchbend(pitchBendEvent.pitchBendValue, position);
		}

		void SheetEventRenderer::handleMetaEvent(const Event &metaEvent) 
		{
			const auto &args = metaEvent.metaArgs;
			const auto &commandName = metaEvent.stringValue;
			try {
				auto &wm = fm::getWerckmeister();
				auto command = wm.solveOrDefault<ACommand>(commandName);
				if (command) {
					auto *usingAnEvent = dynamic_cast<AUsingAnEvent*>(command.get());
					if (usingAnEvent) {
						usingAnEvent->event(metaEvent);
					}
					command->setArguments(args);
					command->execute(ctx_);
					return;
				}
			} catch(const std::exception &ex) {
				FM_THROW(Exception, "failed to process \"" + commandName 
									+"\", \n" + ex.what());
			}	
			catch(...) {
				FM_THROW(Exception, "failed to process \"" + commandName + "\"");
			}
		}
    }
}