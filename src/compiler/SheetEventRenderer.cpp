#include "SheetEventRenderer.h"
#include "spielanweisung/ASpielanweisung.h"
#include "spielanweisung/spielanweisungen.h"
#include "spielanweisung/Vorschlag.h"
#include "modification/AModification.h"
#include <com/werckmeister.hpp>
#include <compiler/commands/ACommand.h>
#include <compiler/commands/AUsingAnEvent.h>
#include <compiler/Warning.hpp>
#include "IEventLogger.h"
#include <compiler/Instrument.h>
#include <list>

namespace compiler
{
	namespace
	{
		void addTagsRecursive(documentModel::Event &event, std::list<com::String>& tags)
		{
			event.tags.insert(tags.begin(), tags.end());
			std::list<com::String> tagsCopy = tags;
			tags.insert(tags.begin(), event.tags.begin(), event.tags.end());
			for (auto &groupedEvent : event.eventGroup)
			{
				addTagsRecursive(groupedEvent, tags);
			}
			tags.swap(tagsCopy);
		}

		using namespace documentModel;
		template <int EventType>
		bool renderEvent(SheetEventRenderer *renderer, const Event *ev)
		{
			return false;
		}

		template <>
		bool renderEvent<Event::Note>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderEvent(*ev);
			return true;
		}

		template <>
		bool renderEvent<Event::Degree>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderDegree(*ev);
			return true;
		}

		template <>
		bool renderEvent<Event::TiedDegree>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderDegree(*ev);
			return true;
		}

		template <>
		bool renderEvent<Event::TiedNote>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderEvent(*ev);
			return true;
		}

		template <>
		bool renderEvent<Event::PitchBend>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderPitchBendEvent(*ev);
			return true;
		}

		template <>
		bool renderEvent<Event::EOB>(SheetEventRenderer *renderer, const Event *ev)
		{
			auto ctx = renderer->context();
			ctx->newBar();
			return true;
		}

		template <>
		bool renderEvent<Event::Rest>(SheetEventRenderer *renderer, const Event *ev)
		{
			auto ctx = renderer->context();
			ctx->rest(ev->duration);
			return true;
		}

		template <>
		bool renderEvent<Event::Chord>(SheetEventRenderer *renderer, const Event *chordEv)
		{
			auto ctx = renderer->context();
			ctx->currentChordEvent(*chordEv);
			ctx->seek(chordEv->duration);
			return true;
		}
		template <>
		bool renderEvent<Event::Meta>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->handleMetaEvent(*ev);
			return true;
		}
		template <>
		bool renderEvent<Event::Phrase>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderPhrase(*ev);
			return true;
		}
		template <>
		bool renderEvent<Event::TiedPhrase>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderPhrase(*ev);
			return true;
		}
		template <>
		bool renderEvent<Event::Controller>(SheetEventRenderer *renderer, const Event *ev)
		{
			renderer->renderControllerEvent(*ev);
			return true;
		}		
		//////////////////////////////////////////////////
		template <int EventId>
		bool renderEventUnrolled(SheetEventRenderer *renderer, const Event *ev)
		{
			if (ev->type == EventId)
			{
				return renderEvent<EventId>(renderer, ev);
			}
			return renderEventUnrolled<EventId + 1>(renderer, ev);
		}
		template <>
		bool renderEventUnrolled<Event::NumEvents>(SheetEventRenderer *renderer, const Event *ev)
		{
			return false;
		}

		void _addEvent(SheetEventRenderer *renderer, const Event *ev)
		{
			renderEventUnrolled<0>(renderer, ev);
		}

		template <>
		bool renderEvent<Event::Group>(SheetEventRenderer *renderer, const Event *ev)
		{
			const com::Ticks eventDuration = ev->duration;
			if (eventDuration == 0)
			{
				return true;
			}
			com::Ticks totalGroupEventDuration = 0;
			for (const auto &groupedEvent : ev->eventGroup)
			{
				totalGroupEventDuration += groupedEvent.duration;
			}
			if (totalGroupEventDuration == 0)
			{
				return true;
			}
			const com::Ticks div = totalGroupEventDuration / eventDuration;
			for (const auto &groupedEvent : ev->eventGroup)
			{
				Event copy = groupedEvent;
				copy.duration /= div;
				renderer->addEvent(copy);
			}
			return true;
		}
	}

	std::shared_ptr<ASheetEventRenderer> SheetEventRenderer::createNewSheetEventRenderer(IContextPtr ctx)
	{
		return std::make_shared<SheetEventRenderer>(ctx, compilerVisitor_, logger_, definitionServer_);
	}

	void SheetEventRenderer::addEvent(const Event &_ev)
	{
		auto meta = ctx_->voiceMetaData();
		auto evCopy = _ev;
		evCopy.tags.insert(meta->tags.begin(), meta->tags.end());
		compilerVisitor_->visit(ctx_.get(), evCopy);
		++(meta->eventCount);
		try
		{
			ctx_->warningHandler(std::bind(&SheetEventRenderer::onWarning, this, std::placeholders::_1, evCopy));
			_addEvent(this, &evCopy);
			ctx_->warningHandler(nullptr);
		}
		catch (com::Exception &ex)
		{
			ex << ex_sheet_source_info(evCopy);
			throw;
		}
	}

	void SheetEventRenderer::onWarning(const com::String &message, const Event &event)
	{
		IEventLoggerPtr eventLogger = std::dynamic_pointer_cast<IEventLogger>(logger_);
		if (!eventLogger)
		{
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
		if (meta->expressionPlayedOnce != com::expression::Default)
		{
			tmpExpression = meta->expression;
			meta->expression = meta->expressionPlayedOnce;
			meta->expressionPlayedOnce = com::expression::Default;
		}
		ev.velocity = ctx_->velocity();
		AModification::Events events = {ev};
		auto sanweis = ctx_->spielanweisung();
		if (sanweis)
		{
			sanweis->perform(ctx_, events);
		}
		for (auto mod : meta->modifications)
		{
			mod->perform(ctx_, events);
		}
		for (auto mod : meta->modificationsOnce)
		{
			mod->perform(ctx_, events);
		}
		auto instrument = ctx_->currentInstrumentDef();
		if (instrument)
		{
			instrument->renderEvents(this, events);
		}
		else
		{
			for (const auto &event : events)
			{
				if (event.isPitchBend())
				{
					renderPitchBendEvent(event);
				}
				else if(event.isController())
				{
					renderControllerEvent(event);
				}
				else
				{
					renderEventPitches(event);
				}
			}
		}
		meta->modificationsOnce.clear();
		meta->expression = tmpExpression;
		ctx_->seek(ev.duration);
	}

	void SheetEventRenderer::renderEventPitches(const Event &ev)
	{
		auto ctx = context();
		ctx->seek(ev.offset);
		const auto &pitches = ev.pitches;
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
		auto absolutePosition = meta->position + static_cast<com::Ticks>(pitchBendEvent.offset);
		ctx->renderPitchbend(pitchBendEvent.pitchBendValue, absolutePosition);
	}

	void SheetEventRenderer::renderControllerEvent(const Event &controllerEvent)
	{
		auto ctx = context();
		auto meta = ctx->voiceMetaData();
		auto relativePosition = static_cast<com::Ticks>(controllerEvent.offset);
		ctx->setContinuousController(controllerEvent.controllerNumber, controllerEvent.controllerValue, relativePosition);
	}

	void SheetEventRenderer::handleMetaEvent(const Event &metaEvent)
	{
		const auto &args = metaEvent.metaArgs;
		const auto &commandName = metaEvent.stringValue;
		try
		{
			auto &wm = com::getWerckmeister();
			auto command = wm.solveOrDefault<ACommand>(commandName);
			if (!command)
			{
				FM_THROW(Exception, "command not found: " + commandName);
			}
			auto *usingAnEvent = dynamic_cast<AUsingAnEvent *>(command.get());
			if (usingAnEvent)
			{
				usingAnEvent->event(metaEvent);
			}
			command->setArguments(args);
			command->execute(ctx_);
		}
		catch (const std::exception &ex)
		{
			FM_THROW(Exception, "failed to process \"" + commandName + "\", \n" + ex.what());
		}
		catch (...)
		{
			FM_THROW(Exception, "failed to process \"" + commandName + "\"");
		}
	}
	void SheetEventRenderer::renderPhrase(const documentModel::Event &phraseEvent)
	{
		PhraseNames callStack;
		renderPhraseImpl(phraseEvent, callStack);
	}

	void SheetEventRenderer::renderPhraseImpl(const documentModel::Event &phraseEvent, PhraseNames &phraseNamesCallStack)
    {
		bool isTiedEvent = phraseEvent.tiedDurationTotal > 0;
		bool isInBetweenTiedEvents = isTiedEvent && phraseEvent.tiedDuration != phraseEvent.tiedDurationTotal;
		if (isInBetweenTiedEvents)
		{
			ctx_->seek(phraseEvent.duration);
			return;
		}
        auto phraseName = phraseEvent.phraseName();
		compilerVisitor_->beginRenderPhrase(phraseName);
        logger_->babble(WMLogLambda(log << "render phrase '" << phraseName << "'"));
        auto phraseInfo = definitionServer_->getPhrase(phraseName);
        if(phraseInfo.events == nullptr)
        {
            FM_THROW(Exception, "phrase not found: " + phraseName);
        }
        const auto &phraseEvents = *phraseInfo.events;
        logger_->babble(WMLogLambda(log << "phrase found: " << phraseEvents.size() << " events"));
		com::Ticks phraseEventLength = isTiedEvent ? phraseEvent.tiedDurationTotal : phraseEvent.duration;
		com::Ticks totalRenderLength =  phraseEventLength;
        std::list<com::String> phraseTags(phraseEvent.tags.begin(), phraseEvent.tags.end());
		bool noTimeConsumingEventsRendered = true;
		for(const auto &event : phraseEvents)
        {
            auto copy = event;
			noTimeConsumingEventsRendered &= !event.isTimeConsuming();
			if (phraseEvent.isPhraseStrechedPlayback)
			{
            	copy.duration = event.duration * (phraseEventLength / phraseInfo.duration);
			} else 
			{
				if (totalRenderLength < event.duration) {
					copy.duration = totalRenderLength;
				}
				totalRenderLength -= event.duration;
			}
			addTagsRecursive(copy, phraseTags);
			if (event.isPhrase())
			{
				bool isCyclicRecursion = phraseNamesCallStack.find(event.phraseName()) != phraseNamesCallStack.end();
				if (isCyclicRecursion)
				{
					FM_THROW(Exception, "recursion detected in phrase: " + phraseName);
				}
				phraseNamesCallStack.insert(copy.phraseName());
				renderPhraseImpl(copy, phraseNamesCallStack);
				phraseNamesCallStack.erase(copy.phraseName());
			} else {
            	addEvent(copy);
			}
			if (totalRenderLength <= 0) {
				break;
			}
        }
		if (noTimeConsumingEventsRendered == false && phraseEvent.isPhraseStrechedPlayback == false && totalRenderLength > 0) 
		{
			ctx_->seek(totalRenderLength);
		}
		if (isTiedEvent)
		{
			ctx_->seek(-(phraseEventLength - phraseEvent.duration));
		}
		compilerVisitor_->endRenderPhrase(phraseName);
    }
	void SheetEventRenderer::renderDegree(const documentModel::Event &degreeEvent)
	{
		auto chordEvent = ctx_->currentChordEvent();
		bool isFallbackChord = !chordEvent.tags.empty() && (*chordEvent.tags.begin() == "fallback"); 
		documentModel::Event absolutePitch;
		definitionServer_->degreeToAbsoluteNote(ctx_, chordEvent, degreeEvent, absolutePitch, !isFallbackChord);
		addEvent(absolutePitch);
	}
}