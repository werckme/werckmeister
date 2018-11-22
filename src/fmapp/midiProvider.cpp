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

	void MidiProvider::getEvents(fm::Ticks at, Events &out)
	{
		
	}

}