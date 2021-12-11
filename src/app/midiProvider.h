#ifndef MIDI_PROVIDER
#define MIDI_PROVIDER

#include <list>
#include <com/midi.hpp>
#include <map>
#include <climits>
#include <functional>
#include <com/config.hpp>
#include <com/units.hpp>

namespace app {
	namespace {
		const double MINUTE = 60.0;
		const double ONE_SECOND_MILLIS = 1000.0;
	}
	class MidiProvider  {
	public:
		enum { NO_TICK = INT_MAX, INVALID_TRACKID = 0 };
		typedef long TrackId;
		typedef long double Millis;
		struct Event {
			TrackId trackId;
			com::midi::Event event;
		};
		typedef std::unordered_map<TrackId, int> TrackOffsets;
		typedef std::list<Event> Events;
		typedef std::function<bool(com::Ticks, const Event &)> IterateFunction;
		void getEvents(Millis at, Events &out, const TrackOffsets &offsets);
		void iterate(const IterateFunction &f);
		virtual ~MidiProvider() = default;
		void midi(com::midi::MidiPtr midi);
		com::midi::MidiPtr midi() const;
		void reset();
		inline com::Ticks millisToTicks(Millis millis) const {
			return static_cast<com::Ticks>( millis * bpm() * com::PPQ / (MINUTE * ONE_SECOND_MILLIS) );
		}
		inline Millis ticksToMillis(com::Ticks ticks) const {
			return MINUTE * ONE_SECOND_MILLIS * ticks / (bpm() * com::PPQ);
		}
	protected:
		inline com::BPM bpm() const { return std::max(bpm_, 1.0); }
		void bpm(com::BPM bpm) { bpm_ = bpm; }	
		void seek(Millis millis, const TrackOffsets &offsets);
	private:
		com::midi::MidiPtr midi_ = nullptr;
		typedef com::midi::EventContainer::ConstIterator EventIt;
		typedef std::unordered_map<com::midi::TrackPtr, EventIt> TrackEventIts;
		TrackEventIts trackEventIts_;
		EventIt* getEventIt(com::midi::TrackPtr trackPtr);
		com::BPM bpm_ = com::DefaultTempo;
	};

}

#endif