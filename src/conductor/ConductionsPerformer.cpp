#include "ConductionsPerformer.h"
#include <documentModel/Document.h>
#include <com/werckmeister.hpp>
#include "selectors/ISelector.h"
#include <compiler/error.hpp>
#include <com/tools.h>
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>
#include <map>
#include <algorithm>

namespace conductor
{
	namespace
	{
		com::String selNamespace_ = "conductor.sel.";
		com::String declNamespace_ = "conductor.decl.";
		com::midi::Event *findCorrespondingNoteOffEvent(com::midi::EventContainer::Iterator noteOn, com::midi::EventContainer::Iterator end)
		{
			for (auto it = noteOn; it != end; ++it)
			{
				if (it->eventType() != com::midi::NoteOff)
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
		std::pair<com::midi::Event *, com::midi::Event *> findPredecessorOfSamePitch(com::midi::EventContainer::Iterator note, com::midi::EventContainer::Iterator begin)
		{
			com::midi::Event *noteOn = nullptr;
			com::midi::Event *noteOff = nullptr;
			for (auto it = note; it != begin; --it)
			{
				if (it->eventType() != com::midi::NoteOn && it->eventType() != com::midi::NoteOff)
				{
					continue;
				}
				if (it->parameter1() != note->parameter1() || it->channel() != note->channel())
				{
					continue;
				}
				if (it->eventType() == com::midi::NoteOff)
				{
					noteOff = &(*it);
				}
				if (it->eventType() == com::midi::NoteOn)
				{
					noteOn = &(*it);
				}
				if (noteOn && noteOff)
				{
					return std::make_pair(noteOn, noteOff);
				}
			}

			return std::make_pair(nullptr, nullptr);
		}

		inline com::Ticks calculateBarNumber(com::Ticks quarters, TimeSignature signature)
		{
			return (quarters * signature.second) / (4.0 * signature.first);
		}
	}

	void ConductionsPerformer::applyConductions()
	{
		auto eventAndOperations = selectEvents();
		perform(eventAndOperations);
	}

	ConductionsPerformer::Events ConductionsPerformer::findMatches(const documentModel::ConductionSelector &selector) const
	{
		Events result;
		auto &wm = com::getWerckmeister();
		for (auto &track : _midifile->tracks())
		{
			auto it = track->events().container().begin();
			auto end = track->events().container().end();
			auto begin = track->events().container().begin();
			TimeSignature timeSignature = {4, 4};
			com::Ticks signatureChangeBarOffset = 0;
			com::String instrumentName;			
			for (; it != end; ++it)
			{
				if (it->eventType() == com::midi::MetaEvent && it->metaEventType() == com::midi::TimeSignature)
				{
					com::Ticks quarters = it->absPosition() / com::PPQ;
					com::Ticks oldBarNumber = calculateBarNumber(quarters, timeSignature);
					timeSignature = com::midi::Event::MetaGetSignatureValue(it->metaData(), it->metaDataSize());
					com::Ticks newBarNumber = calculateBarNumber(quarters, timeSignature);
					signatureChangeBarOffset += oldBarNumber - newBarNumber;
				}				
				com::midi::Event &event = *it;
				if (!isEventOfInterest(event))
				{
					continue;
				}
				com::Ticks quarters = it->absPosition() / com::PPQ;
				auto selectorImpl = wm.solveOrDefault<ISelector>(selNamespace_ + selector.type);
				if (!selectorImpl)
				{
					FM_THROW(compiler::Exception, "selector not found: " + selector.type);
				}
				EventWithMetaInfo eventWithMetaInfo;
				eventWithMetaInfo.timeSignature = timeSignature;
				eventWithMetaInfo.midiEvent = &event;
				eventWithMetaInfo.unmodifiedOriginalMidiEvent = event;
				eventWithMetaInfo.barNumber = calculateBarNumber(quarters, timeSignature) + signatureChangeBarOffset;
				if (selectorImpl->isMatch(selector.arguments, eventWithMetaInfo))
				{
					auto noteOff = findCorrespondingNoteOffEvent(it, end);
					if (it != begin)
					{
						auto predecessor = findPredecessorOfSamePitch(it - 1, begin);
						eventWithMetaInfo.noteOff = noteOff;
						if (noteOff != nullptr) {
							eventWithMetaInfo.unmodifiedOriginalNoteOff = *noteOff;
						}
						eventWithMetaInfo.predecessorNoteOn = predecessor.first;
						eventWithMetaInfo.predecessorNoteOff = predecessor.second;
					}
					result.emplace_back(eventWithMetaInfo);
				}
			}
		}
		return result;
	}

	ConductionsPerformer::Events ConductionsPerformer::findMatches(const documentModel::ConductionSelector &selector, Events &events) const
	{
		Events result;
		auto &wm = com::getWerckmeister();
		for (auto eventAndMetaInfo : events)
		{
			if (!isEventOfInterest(*eventAndMetaInfo.midiEvent))
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
		auto &wm = com::getWerckmeister();
		auto result = EventsAndDeclarationsCollection();

		int nthRule = 0;
		int totalRules = 0;
		for (const auto &cs : _document->conductionSheets)
		{
			totalRules += (int)cs.rules.size();
		}
		for (const auto &cs : _document->conductionSheets)
		{
			for (auto const &rule : cs.rules)
			{
				++nthRule;
				EventsAndDeclarations newValue;
				result.emplace_back(newValue);
				auto eventsAndDeclarations = &result.back();
				Events matchedMidiEvents;
				for (auto const &selectors : rule.selectorsSet)
				{
					Events matchedSubset;
					for(auto const &selector : selectors)
					{
						try
						{
							if (matchedSubset.empty())
							{
								matchedSubset = findMatches(selector);
							}
							else
							{
								matchedSubset = findMatches(selector, matchedSubset);
							}
							bool selectorDoesentMatch = matchedSubset.empty();
							if (selectorDoesentMatch)
							{
								matchedSubset.clear();
								break;
							}
						}
						catch (com::Exception &ex)
						{
							ex << compiler::ex_sheet_source_info(selector);
							throw;
						}
					}
					std::copy(matchedSubset.begin(), matchedSubset.end(), std::back_inserter(eventsAndDeclarations->events));
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
						double specificity = nthRule; 
						declarationImpl->setDeclarationData(declaration);
						declarationImpl->specificity(specificity);
						eventsAndDeclarations->declarations.push_back(declarationImpl);
					}
					catch (com::Exception &ex)
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
		typedef std::pair<const EventWithMetaInfo *, IDeclarationPtr> DeclarationData;
		std::multimap<const com::midi::Event *, DeclarationData> eventsWithDeclarations;
		// 1.
		for (const auto &eventsAndDeclarations : collection)
		{
			for (const auto &eventAndMetaInfo : eventsAndDeclarations.events)
			{
				for (auto declaration : eventsAndDeclarations.declarations)
				{
					eventsWithDeclarations.insert({eventAndMetaInfo.midiEvent, {&eventAndMetaInfo, declaration}});
				}
			}
		}

		auto it = eventsWithDeclarations.begin();
		while (it != eventsWithDeclarations.end())
		{
			const com::midi::Event *event = it->first;
			auto rangeIts = eventsWithDeclarations.equal_range(event);
			std::vector<DeclarationData> declarationDataContainer;
			declarationDataContainer.reserve(eventsWithDeclarations.count(event));
			for (auto it = rangeIts.first; it != rangeIts.second; ++it)
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
				try
				{
					declaration->perform({
						eventAndMetaInfo.midiEvent,
						eventAndMetaInfo.noteOff,
						eventAndMetaInfo.predecessorNoteOn,
						eventAndMetaInfo.predecessorNoteOff,
						eventAndMetaInfo.unmodifiedOriginalMidiEvent,
						eventAndMetaInfo.unmodifiedOriginalNoteOff
					});
				}
				catch (com::Exception &ex)
				{
					ex << compiler::ex_sheet_source_info(declaration->getDeclarationData());
					throw ex;
				}
			}
			it = rangeIts.second;
		}
	}

	bool ConductionsPerformer::isEventOfInterest(const com::midi::Event &event) const
	{
		return event.eventType() == com::midi::NoteOn  
		|| event.eventType() == com::midi::Controller
		|| event.eventType() == com::midi::PitchBend
		|| event.eventType() == com::midi::CuePoint
		|| event.eventType() == com::midi::ChannelAftertouch;
	}
}