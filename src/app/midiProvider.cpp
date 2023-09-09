#include "midiProvider.h"

namespace app
{

	namespace
	{
		template <class TOffsetMap, typename TId>
		int getOffset(const TOffsetMap &map, const TId &id)
		{
			auto it = map.find(id);
			if (it == map.end())
			{
				return 0;
			}
			return it->second;
		}
	}

	void MidiProvider::midi(com::midi::MidiPtr midi)
	{
		midi_ = midi;
	}

	com::midi::MidiPtr MidiProvider::midi() const
	{
		return midi_;
	}

	void MidiProvider::getEvents(Millis millis, Events &out, const TrackOffsets &offsets)
	{
		for (auto track : midi_->ctracks())
		{
			auto trackId = reinterpret_cast<TrackId>(track.get());
			auto offset = getOffset(offsets, trackId);
			auto at = millisToTicks(millis - offset);
			auto end = track->events().end();
			EventIt &it = *getEventIt(track);
			while (it != end)
			{
				auto pos = it->absPosition();
				if (pos > at)
				{
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
		for (auto track : midi_->ctracks())
		{
			auto end = track->events().end();
			EventIt &it = *getEventIt(track);
			while (it != end)
			{
				auto pos = it->absPosition();
				Event ev;
				ev.event = *it;
				ev.trackId = reinterpret_cast<TrackId>(track.get());
				if (!f(pos, ev))
				{
					break;
				}
				++it;
			}
		}
	}

	MidiProvider::EventIt *MidiProvider::getEventIt(com::midi::TrackPtr trackPtr)
	{
		auto it = trackEventIts_.find(trackPtr);
		if (it == trackEventIts_.end())
		{
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
		if (millis > 0)
		{
			millis = std::max(millis - ticksToMillis(1), Millis(0.0));
			getEvents(millis, events, offsets);
		}
	}

}