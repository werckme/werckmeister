#ifndef MIDI_PROVIDER
#define MIDI_PROVIDER

#include "AMidiProvider.h"
#include <list>
#include <fm/midi.hpp>
#include <map>
#include <climits>
#include <functional>

namespace fmapp {

	class MidiProvider : public AMidiProvider {
	public:
		enum { NO_TICK = INT_MAX };
		typedef long TrackId;
		struct Event {
			TrackId trackId;
			fm::midi::Event event;
		};
		typedef std::list<Event> Events;
		void getEvents(fm::Ticks at, Events &out);
		virtual ~MidiProvider() = default;
		void midi(fm::midi::MidiPtr midi);
		fm::midi::MidiPtr midi() const;
		virtual void reset() override;
	protected:
		virtual void seek(fm::Ticks ticks) override;
	private:
		fm::midi::MidiPtr midi_ = nullptr;
		typedef fm::midi::EventContainer::ConstIterator EventIt;
		typedef std::unordered_map<fm::midi::TrackPtr, EventIt> TrackEventIts;
		TrackEventIts trackEventIts_;
		EventIt* getEventIt(fm::midi::TrackPtr trackPtr);
	};

}

#endif