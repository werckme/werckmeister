#include "ConductionsPerformer.h"
#include <sheet/Document.h>
#include <fm/werckmeister.hpp>
#include "selectors/ISelector.h"
#include <compiler/error.hpp>

namespace sheet
{
	namespace conductor
	{
		void ConductionsPerformer::applyConductions()
		{
			auto eventAndOperations = selectEvents();
			perform(eventAndOperations);
			_midifile->seal();
		}

		std::vector<fm::midi::Event*> ConductionsPerformer::findMatches(const sheet::ConductionSelector& selector) const
		{
			std::vector<fm::midi::Event*> result;
			auto& wm = fm::getWerckmeister();
			for (auto& track : _midifile->tracks())
			{
				for (auto& event : track->events().container())
				{
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
						
						result.push_back(&event);
					}
				}
			}
			return result;
		}

		std::vector<fm::midi::Event*> ConductionsPerformer::findMatches(const sheet::ConductionSelector& selector, EventsAndDeclarations::Events& events) const
		{
			std::vector<fm::midi::Event*> result;
			auto& wm = fm::getWerckmeister();
			for (auto event : events)
			{
				if (!isEventOfInterest(*event))
				{
					continue;
				}
				auto selectorImpl = wm.solveOrDefault<ISelector>(selector.type);
				if (!selectorImpl)
				{
					FM_THROW(compiler::Exception, "selector not found: " + selector.type);
				}
				if (selectorImpl->isMatch(selector.arguments, *event))
				{

					result.push_back(event);
				}
			}
			return result;
		}

		ConductionsPerformer::EventsAndDeclarationsCollection ConductionsPerformer::selectEvents() const
		{
			auto &wm = fm::getWerckmeister();
			auto result = EventsAndDeclarationsCollection();

			for (const auto &cs : _document->conductionSheets)
			{
				for (auto const &rule : cs.rules)
				{
					EventsAndDeclarations newValue;
					result.emplace_back(newValue);
					auto eventsAndDeclarations = &result.back();
					for (auto const &selector : rule.selectors)
					{
						EventsAndDeclarations::Events matchedMidiEvents;
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
						for (const auto& declaration : rule.declarations) {
							auto declarationImpl = wm.solveOrDefault<IDeclaration>(declaration.property);
							if (!declarationImpl) {
								FM_THROW(compiler::Exception, "declaration not found: " + declaration.property);
							}
							eventsAndDeclarations->declarations.push_back(declarationImpl);
						}
					}
				}
			}
			return result;
		}

		void ConductionsPerformer::perform(const EventsAndDeclarationsCollection& collection) const
		{
			for (const auto& eventsAndDeclarations : collection) {
				for (auto event : eventsAndDeclarations.events) {
					for (auto declaration : eventsAndDeclarations.declarations) {
						declaration->perform(*event);
					}
				}
			}
		}

		bool ConductionsPerformer::isEventOfInterest(const fm::midi::Event &event) const
		{
			return event.eventType() == fm::midi::NoteOn;
		}
	}
}