#include "ConductionsPerformer.h"
#include <sheet/Document.h>
#include <fm/werckmeister.hpp>
#include "selectors/ISelector.h"
#include <compiler/error.hpp>
#include <fm/tools.h>
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>
#include <map>
#include <algorithm>

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
					if (it->parameter1() == noteOn->parameter1() && it->channel() == noteOn->channel())
					{
						return &(*it);
					}
				}
				return nullptr;
			}

			/// finds predecsessor note on and off of the same picth and chanel as the given note, or null
			std::pair<fm::midi::Event*, fm::midi::Event*> findPredecessorOfSamePitch(fm::midi::EventContainer::Iterator note, fm::midi::EventContainer::Iterator begin)
			{
				auto it = note;
				fm::midi::Event* noteOn = nullptr;
				fm::midi::Event* noteOff = nullptr;
				do
				{
					if (it->eventType() != fm::midi::NoteOn && it->eventType() != fm::midi::NoteOff)
					{
						continue;
					}
					if (it->parameter1() != note->parameter1() || it->channel() != note->channel())
					{
						continue;
					}
					if (it->eventType() == fm::midi::NoteOff) 
					{
						noteOff = &(*it);
					}
					if (it->eventType() == fm::midi::NoteOn) 
					{
						noteOn = &(*it);
					}
					if (noteOn && noteOff) {
						return std::make_pair(noteOn, noteOff);
					}
				} while (it-- != begin);	

				return std::make_pair(nullptr, nullptr);
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
		}

		ConductionsPerformer::Events ConductionsPerformer::findMatches(const sheet::ConductionSelector &selector) const
		{
			Events result;
			auto &wm = fm::getWerckmeister();
			for (auto &track : _midifile->tracks())
			{
				auto it = track->events().container().begin();
				auto end = track->events().container().end();
				auto begin = track->events().container().begin();
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
						if (it != begin) {
							auto predecessor = findPredecessorOfSamePitch(it-1, begin);
							eventWithMetaInfo.noteOff = noteOff;
							eventWithMetaInfo.predecessorNoteOn = predecessor.first;
							eventWithMetaInfo.predecessorNoteOff = predecessor.second;
						}
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

			int nthRule = 0;
			int totalRules = 0;
			for (const auto &cs : _document->conductionSheets) 
			{
				totalRules += cs.rules.size();
			}
			for (const auto &cs : _document->conductionSheets)
			{
				for (auto const &rule : cs.rules)
				{
					++nthRule;
					EventsAndDeclarations newValue;
					result.emplace_back(newValue);
					auto eventsAndDeclarations = &result.back();
					for (auto const &selector : rule.selectors)
					{
						try 
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
						catch (fm::Exception& ex)
						{
							ex << compiler::ex_sheet_source_info(selector);
							throw;
						}
					}
					for (const auto &declaration : rule.declarations)
					{
						try 
						{
							auto declarationImpl = wm.solveOrDefault<IDeclaration>(declNamespace_ + declaration.property);
							if (!declarationImpl)
							{
								FM_THROW(compiler::Exception, "declaration not found: " + declaration.property);
							}
							double orderScore = nthRule / double((totalRules + 1)); // shold not be >= 1
							double specificity = double(rule.selectors.size()) + orderScore;
							declarationImpl->setDeclarationData(declaration);
							declarationImpl->specificity(specificity);
							eventsAndDeclarations->declarations.push_back(declarationImpl);
						}
						catch (fm::Exception& ex)
						{
							ex << compiler::ex_sheet_source_info(declaration);
							throw;
						}
					}
				}
			}
			return result;
		}

		void ConductionsPerformer::perform(const EventsAndDeclarationsCollection &collection) const
		{
			// 1. collect all declarations per note on event
			// 2. order ascending by declaration prio (higher prio values comes last)
			// 3. perform
			typedef std::pair<const EventWithMetaInfo*, IDeclarationPtr> DeclarationData;
			std::multimap<const fm::midi::Event*, DeclarationData> eventsWithDeclarations;
			// 1.
			for (const auto &eventsAndDeclarations : collection)
			{
				for (const auto &eventAndMetaInfo : eventsAndDeclarations.events)
				{
					for (auto declaration : eventsAndDeclarations.declarations)
					{
						eventsWithDeclarations.insert({eventAndMetaInfo.noteOn, {&eventAndMetaInfo, declaration}});
					}
				}
			}

			auto it = eventsWithDeclarations.begin();
			while(it != eventsWithDeclarations.end()) 
			{
				const fm::midi::Event* event = it->first;
				auto rangeIts = eventsWithDeclarations.equal_range(event);
				std::vector<DeclarationData> declarationDataContainer;
				declarationDataContainer.reserve(eventsWithDeclarations.count(event));
				for(auto it = rangeIts.first; it != rangeIts.second; ++it) 
				{
					declarationDataContainer.push_back(it->second);
				}
				// 2.
				std::sort(declarationDataContainer.begin(), declarationDataContainer.end(), [this](const auto &a, const auto &b) -> bool
				{
					IDeclarationPtr aDecl = a.second;
					IDeclarationPtr bDecl = b.second;
					return aDecl->specificity() < bDecl->specificity();
				});
				// 3.
				for (const DeclarationData &declarationData : declarationDataContainer)
				{
					IDeclarationPtr declaration = declarationData.second;
					const EventWithMetaInfo &eventAndMetaInfo = *declarationData.first;
					// _logger->error(WMLogLambda(log << eventAndMetaInfo.noteOn->absPosition() << " : " << declaration->priority()));
					try 
					{
						declaration->perform({
							eventAndMetaInfo.noteOn, 
							eventAndMetaInfo.noteOff,
							eventAndMetaInfo.predecessorNoteOn,
							eventAndMetaInfo.predecessorNoteOff,
						});
					}
					catch(fm::Exception &ex)
					{
						ex << compiler::ex_sheet_source_info(declaration->getDeclarationData());
						throw ex;
					}
				}
				it = rangeIts.second;
			}
			
		}

		bool ConductionsPerformer::isEventOfInterest(const fm::midi::Event &event) const
		{
			return event.eventType() == fm::midi::NoteOn;
		}
	}
}