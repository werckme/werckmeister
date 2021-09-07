#ifndef CONDUCTIONS_PERFORMER_H
#define CONDUCTIONS_PERFORMER_H

#include "IConductionsPerformer.h"
#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <fm/midi.hpp>
#include <vector>

namespace sheet
{
	namespace conductor
	{
		class ConductionsPerformer : public IConductionsPerformer
		{
		protected:
			struct EventsAndOperations {
				typedef std::vector<fm::midi::Event*> Events;
				Events events;
			};
			typedef std::vector<EventsAndOperations> EventsAndOperationsCollection;
			EventsAndOperationsCollection selectEvents() const;
		public:
			ConductionsPerformer(fm::midi::MidiPtr midifile, sheet::DocumentPtr document) : 
				_midifile(midifile), _document(document) {}
			virtual void applyConductions() override;
		private:
			bool isEventOfInterest(const fm::midi::Event&) const;
			fm::midi::MidiPtr _midifile;
			sheet::DocumentPtr _document;
		};
	}
}

#endif