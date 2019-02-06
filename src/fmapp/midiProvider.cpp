#include "midiProvider.h"

namespace fmapp {

	namespace {
		template<class TOffsetMap, typename TId>
		int getOffset(const TOffsetMap &map, const TId &id) {
			auto it = map.find(id);
			if (it == map.end()) {
				return 0;
			}
			return it->second;
		}
	}

	void MidiProvider::midi(fm::midi::MidiPtr midi)
	{
		midi_ = midi;
	}
	
	fm::midi::MidiPtr MidiProvider::midi() const
	{
		return midi_;
	}

	void MidiProvider::getEvents(Millis millis, Events &out, const TrackOffsets &offsets)
	{
		auto at = millisToTicks(millis);
		for (auto track : midi_->tracks()) {
			auto trackId = reinterpret_cast<TrackId>(track.get());
			auto offset = getOffset(offsets, trackId);
			if (offset != 0) {
				at = millisToTicks(millis + offset);
			}
			auto end = track->events().end();
			EventIt &it = *getEventIt(track);
			while (it != end) {
				auto pos = it->absPosition();
				if (pos > at) {
					break;
				}
				Event ev;
				ev.event = *it;
				ev.trackId = trackId;
				out.push_back(ev);
				++it;
			}
		}
	}

	void MidiProvider::iterate(const IterateFunction &f)
	{
		for (auto track : midi_->tracks()) {
			auto end = track->events().end();
			EventIt &it = *getEventIt(track);
			while (it != end) {
				auto pos = it->absPosition();
				Event ev;
				ev.event = *it;
				ev.trackId = reinterpret_cast<TrackId>(track.get()); 
				if (!f(pos, ev)) {
					break;
				}
				++it;
			}
		}
	}

	MidiProvider::EventIt* MidiProvider::getEventIt(fm::midi::TrackPtr trackPtr)
	{
		auto it = trackEventIts_.find(trackPtr);
		if (it == trackEventIts_.end()) {
			auto eventIt = trackPtr->events().begin();
			it = trackEventIts_.insert({trackPtr, eventIt}).first;
		}
		return &it->second;
	}

	void MidiProvider::reset()
	{
		trackEventIts_.clear();
	}

	void MidiProvider::seek(Millis millis, const TrackOffsets &offsets)
	{
		trackEventIts_.clear();
		Events events;
		auto ticks = millisToTicks(millis);
		if (ticks > 0) {
			// reset iterators
			ticks -= 1;
			getEvents(ticks, events, offsets);
		}
	}

}