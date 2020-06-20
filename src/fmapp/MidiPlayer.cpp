#include "MidiPlayer.h"
#include <fm/config.hpp>
#include <fmapp/os.hpp>
#include <fm/config.hpp>
#include <thread>
#include <iostream>


#define UPDATE_THREAD_SLEEPTIME 70

namespace fmapp {

    void MidiPlayer::listDevices(std::ostream& os)
    {
        auto outputs = _midiPlayerImpl.getOutputs();
        for (const auto &output : outputs) {
            os << output.id << ": " << output.name << std::endl;
        }
    }

    void MidiPlayer::write(sheet::DocumentPtr doc)
    {
        if (state > Stopped) {
            return;
        }
        _logger->babble(WMLogLambda(log << "start playback"));
        _logger->babble(WMLogLambda(log << "midi: " << _midifile->byteSize() << " bytes"));
        execLoop(doc);
    }

    void MidiPlayer::execLoop(sheet::DocumentPtr)
    {
        fm::Ticks begin = 0;
        if (_programOptions->isBeginSet()) {
            begin = _programOptions->getBegin() * fm::PPQ;
        }
        _logger->babble(WMLogLambda(log << "begin at tick: " << begin));
        fm::Ticks end = _midifile->duration();
        _midiPlayerImpl.updateOutputMapping(fm::getConfigServer().getDevices());
        _midiPlayerImpl.midi(_midifile);
        _midiPlayerImpl.play(begin);

        state = Playing;
        bool loop   = _programOptions->isLoopSet();
        fmapp::os::setSigtermHandler([&]{
            state = Stopped;
            _midiPlayerImpl.panic();
        });

#ifdef SIGINT_WORKAROUND
        // std::unique_ptr<fmapp::os::InterProcessMessageQueue> ipcMessageQueue = nullptr;
        // bool isSigintWorkaround = false; // TODO: #126
        // if (isSigintWorkaround) {
        //     ipcMessageQueue = std::make_unique<fmapp::os::InterProcessMessageQueue>();
        // }
#endif
        while (state > Stopped) {
            auto elapsed = _midiPlayerImpl.elapsed();
            std::this_thread::sleep_for( std::chrono::milliseconds(UPDATE_THREAD_SLEEPTIME) );
#ifdef SIGINT_WORKAROUND
            // if (ipcMessageQueue && ipcMessageQueue->sigintReceived()) {
            //     playing = false;
            //     player.panic();
            // }
#endif
            if (elapsed > end) {
                if (!loop) {
                    break;
                }
                _midiPlayerImpl.play(begin);
            }
            visitVisitors(elapsed);
        }
        std::cout << std::endl;
        _midiPlayerImpl.stop();
        _midiPlayerImpl.panic();
        _midiPlayerImpl.Backend:: tearDown();

    }

    void MidiPlayer::visitVisitors(fm::Ticks elapsed)
    {
        for(auto visitor : _loopVisitors.container) {
            visitor->visit(elapsed);
        }
    }

    fm::Ticks MidiPlayer::stop()
    {
        if (state == Stopped) {
            return 0;
        }
        auto position = _midiPlayerImpl.elapsed();
        _midiPlayerImpl.stop();
        state = Stopped;
        return position;
    }
}