#include "midiProvider.h"

namespace fmapp {

	void MidiProvider::midi(fm::midi::MidiPtr midi)
	{
		midi_ = midi;
	}
	
	fm::midi::MidiPtr MidiProvider::midi() const
	{
		return midi_;
	}

	void MidiProvider::getEvents(fm::Ticks at, Events &out, const FilterFunc &filter)
	{
		for (auto track : midi_->tracks()) {
			auto end = track->events().end();
			EventIt &it = *getEventIt(track);
			while (it != end) {
				auto pos = it->absPosition();
				if (pos > at) {
					break;
				}
				if (filter(*it)){
					out.push_back(*it);
				}
				++it;
			}
		}
	}

	void MidiProvider::getEvents(fm::Ticks at, Events &out)
	{
		getEvents(at, out, [](const auto&){return true;});
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

	void MidiProvider::seek(fm::Ticks ticks)
	{
		trackEventIts_.clear();
		Events events;
		if (ticks > 0) {
			// reset iterators
			ticks -= 1;
			getEvents(ticks, events);
		}
	}

}