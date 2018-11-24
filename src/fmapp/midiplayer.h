#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include <fm/config.hpp>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"

#ifdef USE_WINDOWS_MME_TIMER
#include "fmapp/os.hpp"
#else
#include "fmapp/boostTimer.h"
#endif

namespace fmapp {
#ifdef USE_WINDOWS_MME_TIMER
	typedef fmapp::os::MMTimer TimerImpl;
#else
	typedef fmapp::BoostTimer TimerImpl;
#endif
	template<class TBackend, class TMidiProvider, class TimerImpl>
	class MidiplayerClient;

	typedef MidiplayerClient<RtMidiBackend, MidiProvider, TimerImpl> Midiplayer;
	Midiplayer & getMidiplayer();
}
#endif