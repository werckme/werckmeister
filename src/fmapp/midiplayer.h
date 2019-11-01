#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include <fm/config.hpp>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"

#ifdef SHEET_USE_BOOST_TIMER
#include "fmapp/boostTimer.h"
#else
#include "fmapp/os.hpp"
#endif

namespace fmapp {
#ifdef SHEET_USE_BOOST_TIMER
	typedef fmapp::BoostTimer TimerImpl;
#else
	typedef fmapp::os::MMTimer TimerImpl;
#endif
	template<class TBackend, class TMidiProvider, class TimerImpl>
	class MidiplayerClient;

	typedef MidiplayerClient<RtMidiBackend, MidiProvider, TimerImpl> Midiplayer;
	Midiplayer & getMidiplayer();
}
#endif