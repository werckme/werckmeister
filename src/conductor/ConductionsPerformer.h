#ifndef CONDUCTIONS_PERFORMER_H
#define CONDUCTIONS_PERFORMER_H

#include "IConductionsPerformer.h"
#include <com/common.hpp>
#include <forward.hpp>
#include <memory>
#include <com/midi.hpp>
#include <vector>
#include <conductor/declarations/IDeclaration.h>
#include <conductor/selectors/ISelector.h>
#include <com/ILogger.h>

namespace sheet
{
	namespace conductor
	{
		class ConductionsPerformer : public IConductionsPerformer
		{
		public:
			typedef std::vector<EventWithMetaInfo> Events;
			typedef std::vector<IDeclarationPtr> Declarations;
		protected:
			struct EventsAndDeclarations {
				Declarations declarations;
				Events events;
			};
			typedef std::vector<EventsAndDeclarations> EventsAndDeclarationsCollection;
			EventsAndDeclarationsCollection selectEvents() const;
		public:
			ConductionsPerformer(com::midi::MidiPtr midifile, sheet::DocumentPtr document, com::ILoggerPtr logger) : 
				_midifile(midifile), _document(document), _logger(logger) {}
			virtual void applyConductions() override;
		private:
			bool isEventOfInterest(const com::midi::Event&) const;
			Events findMatches(const sheet::ConductionSelector &) const;
			Events findMatches(const sheet::ConductionSelector&, Events&) const;
			void perform(const EventsAndDeclarationsCollection&) const;
			com::midi::MidiPtr _midifile;
			sheet::DocumentPtr _document;
			com::ILoggerPtr _logger;
		};
	}
}

#endif