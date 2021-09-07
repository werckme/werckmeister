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
		}

		ConductionsPerformer::EventsAndOperationsCollection ConductionsPerformer::selectEvents() const
		{
			auto &wm = fm::getWerckmeister();
			auto result = EventsAndOperationsCollection();
			for(auto &track : _midifile->tracks()) {
				for (auto &event : track->events()) {
					if (!isEventOfInterest(event)) {
						continue;
					}
					for (const auto &cs : _document->conductionSheets) {
						for (auto const &rule : cs.rules) {
							for (auto const &selector : rule.selectors) {
								auto selectorImpl = wm.solveOrDefault<ISelector>(selector.type);
								if (!selectorImpl) {
									FM_THROW(compiler::Exception, "selector not found: " + selector.type);
								}
								if (selectorImpl->isMatch(selector.arguments, event)) {
									EventsAndOperations eventsAndOperations;
									//eventsAndOperations.events.push_back(&event);
								}
							}
						}
					}
				}
			}
		}

		bool ConductionsPerformer::isEventOfInterest(const fm::midi::Event& event) const
		{
			return event.eventType() == fm::midi::NoteOn
				|| event.eventType() == fm::midi::NoteOff;
		}
	}
}