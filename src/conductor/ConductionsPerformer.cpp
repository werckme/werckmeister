#include "ConductionsPerformer.h"
#include <sheet/Document.h>
#include <fm/werckmeister.hpp>
#include "selectors/ISelector.h"
#include <compiler/error.hpp>
#include <fm/tools.h>
#include <compiler/context/MidiContext.h>

namespace sheet
{
	namespace conductor
	{
		namespace
		{
			fm::String selNamespace_ = "conductor.sel.";
        	fm::String declNamespace_ = "conductor.decl.";
			fm::midi::Event *findCorrespondingNoteOffEvent(fm::midi::EventContainer::Iterator noteOn, fm::midi::EventContainer::Iterator end)
			{
				auto it = noteOn;
				while (it++ != end)
				{
					if (it->eventType() != fm::midi::NoteOff)
					{
						continue;
					}
					if (it->parameter1() == noteOn->parameter1())
					{
						return &(*it);
					}
				}
				return nullptr;
			}

			inline fm::Ticks calculateBarNumber(fm::Ticks quarters, TimeSignature signature)
			{
				return (quarters * signature.second) / (4.0 * signature.first);
			}
		}

		void ConductionsPerformer::applyConductions()
		{
			auto eventAndOperations = selectEvents();
			perform(eventAndOperations);
			_midifile->seal();
		}

		ConductionsPerformer::Events ConductionsPerformer::findMatches(const sheet::ConductionSelector &selector) const
		{
			Events result;
			auto &wm = fm::getWerckmeister();
			for (auto &track : _midifile->tracks())
			{
				auto it = track->events().container().begin();
				auto end = track->events().container().end();
				TimeSignature timeSignature = {4, 4};
				fm::Ticks signatureChangeBarOffset = 0;
				fm::String instrumentName;
				for (; it != end; ++it)
				{
					if (it->eventType() == fm::midi::MetaEvent && it->metaEventType() == fm::midi::TimeSignature)
					{
						fm::Ticks quarters = it->absPosition() / fm::PPQ;
						fm::Ticks oldBarNumber = calculateBarNumber(quarters, timeSignature);
						timeSignature = fm::midi::Event::MetaGetSignatureValue(it->metaData(), it->metaDataSize());
						fm::Ticks newBarNumber = calculateBarNumber(quarters, timeSignature);
						signatureChangeBarOffset += oldBarNumber - newBarNumber;
					}
					if (it->eventType() == fm::midi::MetaEvent && it->metaEventType() == fm::midi::CustomMetaEvent)
					{
						auto customEvent = fm::midi::Event::MetaGetCustomData(it->metaData(), it->metaDataSize());
						if (customEvent.type == fm::midi::CustomMetaData::SetInstrument)
						{
							instrumentName = fm::String(customEvent.data.begin(), customEvent.data.end());
						}
					}
					fm::midi::Event &event = *it;
					if (!isEventOfInterest(event))
					{
						continue;
					}
					fm::Ticks quarters = it->absPosition() / fm::PPQ;
					auto selectorImpl = wm.solveOrDefault<ISelector>(selNamespace_ + selector.type);
					if (!selectorImpl)
					{
						FM_THROW(compiler::Exception, "selector not found: " + selector.type);
					}
					EventWithMetaInfo eventWithMetaInfo;
					eventWithMetaInfo.timeSignature = timeSignature;
					eventWithMetaInfo.noteOn = &event;
					eventWithMetaInfo.instrumentName = instrumentName;
					eventWithMetaInfo.barNumber = calculateBarNumber(quarters, timeSignature) + signatureChangeBarOffset;
					if (selectorImpl->isMatch(selector.arguments, eventWithMetaInfo))
					{
						auto noteOff = findCorrespondingNoteOffEvent(it, end);
						eventWithMetaInfo.noteOff = noteOff;
						result.emplace_back(eventWithMetaInfo);
					}
				}
			}
			return result;
		}

		ConductionsPerformer::Events ConductionsPerformer::findMatches(const sheet::ConductionSelector &selector, Events &events) const
		{
			Events result;
			auto &wm = fm::getWerckmeister();
			for (auto eventAndMetaInfo : events)
			{
				if (!isEventOfInterest(*eventAndMetaInfo.noteOn))
				{
					continue;
				}
				auto selectorImpl = wm.solveOrDefault<ISelector>(selNamespace_ + selector.type);
				if (!selectorImpl)
				{
					FM_THROW(compiler::Exception, "selector not found: " + selector.type);
				}
				if (selectorImpl->isMatch(selector.arguments, eventAndMetaInfo))
				{

					result.push_back(eventAndMetaInfo);
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
						Events matchedMidiEvents;
						if (eventsAndDeclarations->events.empty())
						{
							matchedMidiEvents = findMatches(selector);
						}
						else
						{
							matchedMidiEvents = findMatches(selector, eventsAndDeclarations->events);
						}
						if (matchedMidiEvents.empty())
						{
							continue;
						}
						eventsAndDeclarations->events.swap(matchedMidiEvents);
					}
					for (const auto &declaration : rule.declarations)
					{
						auto declarationImpl = wm.solveOrDefault<IDeclaration>(declNamespace_ + declaration.property);
						if (!declarationImpl)
						{
							FM_THROW(compiler::Exception, "declaration not found: " + declaration.property);
						}
						declarationImpl->setDeclarationData(declaration);
						eventsAndDeclarations->declarations.push_back(declarationImpl);
					}
				}
			}
			return result;
		}

		void ConductionsPerformer::perform(const EventsAndDeclarationsCollection &collection) const
		{
			for (const auto &eventsAndDeclarations : collection)
			{
				for (auto eventAndMetaInfo : eventsAndDeclarations.events)
				{
					for (auto declaration : eventsAndDeclarations.declarations)
					{
						declaration->perform(eventAndMetaInfo.noteOn, eventAndMetaInfo.noteOff);
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