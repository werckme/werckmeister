#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"

namespace fmapp {
	template<class TBackend, class TMidiProvider>
	class MidiplayerClient;

	typedef MidiplayerClient<RtMidiBackend, MidiProvider> Midiplayer;
	Midiplayer & getMidiplayer();
}
#endif