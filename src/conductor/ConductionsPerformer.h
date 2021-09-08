#ifndef CONDUCTIONS_PERFORMER_H
#define CONDUCTIONS_PERFORMER_H

#include "IConductionsPerformer.h"
#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <fm/midi.hpp>
#include <vector>
#include <conductor/declarations/IDeclaration.h>

namespace sheet
{
	namespace conductor
	{
		class ConductionsPerformer : public IConductionsPerformer
		{
		protected:
			struct EventsAndDeclarations {
				typedef std::vector<fm::midi::Event*> Events;
				typedef std::vector<IDeclarationPtr> Declarations;
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
			std::vector<fm::midi::Event*> findMatches(const sheet::ConductionSelector &selector) const;
			void perform(const EventsAndDeclarationsCollection&) const;
			fm::midi::MidiPtr _midifile;
			sheet::DocumentPtr _document;
		};
	}
}

#endif