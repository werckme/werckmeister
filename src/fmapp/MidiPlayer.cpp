#include "MidiPlayer.h"
#include <fm/config.hpp>
#include <fmapp/os.hpp>
#include <fm/config.hpp>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"
#include <thread>
#include <iostream>


#define UPDATE_THREAD_SLEEPTIME 70

// #ifdef WIN32
// #include "fmapp/os_win_ipc_kill_handler.hpp"
// #define SIGINT_WORKAROUND
// #endif


#ifdef SHEET_USE_BOOST_TIMER
#include "fmapp/boostTimer.h"
#else
#include "fmapp/os.hpp"
#endif

#ifdef SHEET_USE_BOOST_TIMER
	typedef fmapp::BoostTimer TimerImpl;
#else
	typedef fmapp::os::MMTimer TimerImpl;
#endif

namespace {
	typedef fmapp::MidiplayerClient<fmapp::RtMidiBackend, fmapp::MidiProvider, TimerImpl> MidiplayerImpl;
	MidiplayerImpl midiPlayerImpl;
}


namespace fmapp {
    void MidiPlayer::write(sheet::DocumentPtr doc)
    {
        _logger->babble(WMLogLambda(log << "start playback"));
        execLoop(doc);
    }

    void MidiPlayer::execLoop(sheet::DocumentPtr)
    {
        fm::Ticks begin = 0; // TODO: #126
        fm::Ticks end = _midifile->duration();
        midiPlayerImpl.updateOutputMapping(fm::getConfigServer().getDevices());
        midiPlayerImpl.midi(_midifile);
        midiPlayerImpl.play(begin);

        bool playing = true;
        bool watch = false; // TODO: #126
        bool loop   = false; // TODO: #126
        //Timestamps timestamps;
        // hasChanges(document, timestamps);	// init timestamps
        // updateLastChangedTimestamp();
        //auto inputfile = settings.getInput();
        fmapp::os::setSigtermHandler([&]{
            playing = false;
            midiPlayerImpl.panic();
        });

#ifdef SIGINT_WORKAROUND
        std::unique_ptr<fmapp::os::InterProcessMessageQueue> ipcMessageQueue = nullptr;
        bool isSigintWorkaround = false // TODO: #126
        if (isSigintWorkaround) {
            ipcMessageQueue = std::make_unique<fmapp::os::InterProcessMessageQueue>();
        }
#endif
#ifdef SHEET_USE_BOOST_TIMER
        std::thread boost_asio_([] {
            fmapp::BoostTimer::io_run();
        });
#endif
        while (playing) {
            auto elapsed = midiPlayerImpl.elapsed();
            std::this_thread::sleep_for( std::chrono::milliseconds(UPDATE_THREAD_SLEEPTIME) );
#ifdef SIGINT_WORKAROUND
            if (ipcMessageQueue && ipcMessageQueue->sigintReceived()) {
                playing = false;
                player.panic();
            }
#endif
            // if (watch) {
            //     if (hasChanges(document, timestamps)) {
            //         try {
            //             updatePlayer(player, inputfile);
            //             updateLastChangedTimestamp();
            //         } catch(...) {
            //             player.panic();
            //             break;
            //         }
            //     }
            // }
            if (elapsed > end) {
                if (!loop) {
                    break;
                }
                midiPlayerImpl.play(begin);
            }
            visitVisitors(elapsed);
        }
        std::cout << std::endl;
        midiPlayerImpl.stop();
        midiPlayerImpl.panic();

    #ifdef SHEET_USE_BOOST_TIMER
        fmapp::BoostTimer::io_stop();
        boost_asio_.join();
    #endif

        midiPlayerImpl.Backend:: tearDown();

    }

    void MidiPlayer::visitVisitors(fm::Ticks elapsed)
    {
        for(auto visitor : _loopVisitors.container) {
            visitor->visit(elapsed);
        }
    }
}