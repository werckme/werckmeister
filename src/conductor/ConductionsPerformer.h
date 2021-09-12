#ifndef CONDUCTIONS_PERFORMER_H
#define CONDUCTIONS_PERFORMER_H

#include "IConductionsPerformer.h"
#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <fm/midi.hpp>
#include <vector>
#include <conductor/declarations/IDeclaration.h>
#include <conductor/selectors/ISelector.h>

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
			ConductionsPerformer(fm::midi::MidiPtr midifile, sheet::DocumentPtr document) : 
				_midifile(midifile), _document(document) {}
			virtual void applyConductions() override;
		private:
			bool isEventOfInterest(const fm::midi::Event&) const;
			Events findMatches(const sheet::ConductionSelector &) const;
			Events findMatches(const sheet::ConductionSelector&, Events&) const;
			void perform(const EventsAndDeclarationsCollection&) const;
			fm::midi::MidiPtr _midifile;
			sheet::DocumentPtr _document;
		};
	}
}

#endif