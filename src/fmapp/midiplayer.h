#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"
#include "fmapp/os.hpp"

namespace fmapp {
	typedef fmapp::os::MMTimer TimerImpl;
	template<class TBackend, class TMidiProvider, class TimerImpl>
	class MidiplayerClient;

	typedef MidiplayerClient<RtMidiBackend, MidiProvider, TimerImpl> Midiplayer;
	Midiplayer & getMidiplayer();
}
#endif