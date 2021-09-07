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
			_midifile->seal();
		}

		ConductionsPerformer::EventsAndOperationsCollection ConductionsPerformer::selectEvents() const
		{
			auto &wm = fm::getWerckmeister();
			auto result = EventsAndOperationsCollection();

			for (const auto &cs : _document->conductionSheets)
			{
				for (auto const &rule : cs.rules)
				{
					for (auto const &selector : rule.selectors)
					{
						EventsAndOperations* eventsAndOperations = nullptr;
						for (auto &track : _midifile->tracks())
						{
							for (auto &event : track->events().container())
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
									if (eventsAndOperations == nullptr) {
										EventsAndOperations newValue;
										result.emplace_back(newValue);
										eventsAndOperations = &result.back();
									}
									eventsAndOperations->events.push_back(&event);
								}
							}
						}
					}
				}
			}
			return result;
		}

		bool ConductionsPerformer::isEventOfInterest(const fm::midi::Event &event) const
		{
			return event.eventType() == fm::midi::NoteOn || event.eventType() == fm::midi::NoteOff;
		}
	}
}