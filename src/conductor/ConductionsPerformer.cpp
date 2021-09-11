#include "ConductionsPerformer.h"
#include <sheet/Document.h>
#include <fm/werckmeister.hpp>
#include "selectors/ISelector.h"
#include <compiler/error.hpp>
#include <fm/tools.h>

namespace sheet
{
	namespace conductor
	{
		namespace 
		{
			fm::midi::Event* findCorrespondingNoteOffEvent(fm::midi::EventContainer::Iterator noteOn, fm::midi::EventContainer::Iterator end)
			{
				auto it = noteOn;
				while (it++ != end) {
					if (it->eventType() != fm::midi::NoteOff) {
						continue;
					}
					if (it->parameter1() == noteOn->parameter1()) {
						return &(*it);
					}
				}
				return nullptr;
			}
		}

		void ConductionsPerformer::applyConductions()
		{
			auto eventAndOperations = selectEvents();
			perform(eventAndOperations);
			_midifile->seal();
		}

		ConductionsPerformer::Events ConductionsPerformer::findMatches(const sheet::ConductionSelector& selector) const
		{
			Events result;
			auto& wm = fm::getWerckmeister();
			for (auto& track : _midifile->tracks())
			{
				auto it = track->events().container().begin();
				auto end = track->events().container().end();
				for (; it != end; ++it)
				{
					fm::midi::Event &event = *it;
					if (!isEventOfInterest(event))
					{
						continue;
					}
					auto selectorImpl = wm.solveOrDefault<ISelector>(selector.type);
					if (!selectorImpl)
					{
						FM_THROW(compiler::Exception, "selector not found: " + selector.type);
					}
					if (selectorImpl->isMatch(selector.arguments, event))
					{
						auto noteOff = findCorrespondingNoteOffEvent(it, end);
						result.push_back(std::make_pair(&event, noteOff));
					}
				}
			}
			return result;
		}

		ConductionsPerformer::Events ConductionsPerformer::findMatches(const sheet::ConductionSelector& selector, Events& events) const
		{
			Events result;
			auto& wm = fm::getWerckmeister();
			for (auto noteOnAndOffEvent : events)
			{
				if (!isEventOfInterest(*noteOnAndOffEvent.first))
				{
					continue;
				}
				auto selectorImpl = wm.solveOrDefault<ISelector>(selector.type);
				if (!selectorImpl)
				{
					FM_THROW(compiler::Exception, "selector not found: " + selector.type);
				}
				if (selectorImpl->isMatch(selector.arguments, *noteOnAndOffEvent.first))
				{

					result.push_back(noteOnAndOffEvent);
				}
			}
			return result;
		}

		ConductionsPerformer::EventsAndDeclarationsCollection ConductionsPerformer::selectEvents() const
		{
			auto& wm = fm::getWerckmeister();
			auto result = EventsAndDeclarationsCollection();

			for (const auto& cs : _document->conductionSheets)
			{
				for (auto const& rule : cs.rules)
				{
					EventsAndDeclarations newValue;
					result.emplace_back(newValue);
					auto eventsAndDeclarations = &result.back();
					for (auto const& selector : rule.selectors)
					{
						Events matchedMidiEvents;
						if (eventsAndDeclarations->events.empty()) {
							matchedMidiEvents = findMatches(selector);
						}
						else {
							matchedMidiEvents = findMatches(selector, eventsAndDeclarations->events);
						}
						if (matchedMidiEvents.empty()) {
							continue;
						}
						eventsAndDeclarations->events.swap(matchedMidiEvents);
					}
					for (const auto& declaration : rule.declarations) {
						auto declarationImpl = wm.solveOrDefault<IDeclaration>(declaration.property);
						if (!declarationImpl) {
							FM_THROW(compiler::Exception, "declaration not found: " + declaration.property);
						}
						declarationImpl->declaration = declaration;
						eventsAndDeclarations->declarations.push_back(declarationImpl);
					}
				}
			}
			return result;
		}

		void ConductionsPerformer::perform(const EventsAndDeclarationsCollection& collection) const
		{
			for (const auto& eventsAndDeclarations : collection) {
				for (auto noteOnAndOff : eventsAndDeclarations.events) {
					for (auto declaration : eventsAndDeclarations.declarations) {
						declaration->perform(noteOnAndOff.first, noteOnAndOff.second);
					}
				}
			}
		}

		bool ConductionsPerformer::isEventOfInterest(const fm::midi::Event& event) const
		{
			return event.eventType() == fm::midi::NoteOn;
		}
	}
}