#include "MidiPlayer.h"
#include <com/config.hpp>
#include <app/os.hpp>
#include <com/config.hpp>
#include <thread>
#include <iostream>
#include <algorithm>
#include "FluidSynthBackend.h"
#include "RTMidiBackend.h"

#define UPDATE_THREAD_SLEEPTIME 70

#ifdef WIN32
#include <app/os_win_ipc_kill_handler.hpp>
#define SIGINT_WORKAROUND
#endif

namespace app
{

    void MidiPlayer::listDevices(std::ostream &os)
    {
        auto outputs = _midiPlayerImpl.getOutputs();
        for (const auto &output : outputs)
        {
            os << output.id << ": " << output.name << std::endl;
        }
    }

    void MidiPlayer::write(documentModel::DocumentPtr doc)
    {
        if (state > Stopped)
        {
            return;
        }
        _logger->babble(WMLogLambda(log << "start playback"));
        _logger->babble(WMLogLambda(log << "midi: " << _midifile->byteSize() << " bytes"));
        execLoop(doc);
    }

    void MidiPlayer::initMidiBackends()
    {
        _logger->babble(WMLogLambda(log << "init rtmidi backend"));
        _midiPlayerImpl.addBackend(std::make_shared<RtMidiBackend>());
        _logger->babble(WMLogLambda(log << "init fluidsynth backend"));
        _midiPlayerImpl.addBackend(std::make_shared<FluidSynthBackend>());
    }

    void MidiPlayer::execLoop(documentModel::DocumentPtr)
    {
        com::Ticks resume = 0;
        com::Ticks begin = 0;
        com::Ticks end = _midifile->duration();
        if (_programOptions->getResumeAtPosition() > 0)
        {
            resume = _programOptions->getResumeAtPosition() * com::PPQ;
            _programOptions->setResumeAtPosition(0);
        }
        initFluidSynthInstances();
        _midiPlayerImpl.updateOutputMapping(com::getConfigServer().getDevices());
        _midiPlayerImpl.midi(_midifile);
        _midiPlayerImpl.play(resume > 0 ? resume : begin);

        state = Playing;
        bool loop = _programOptions->isLoopSet();
        app::os::setSigtermHandler([&]
                                   {
                                       state = Stopped;
                                       _midiPlayerImpl.panic();
                                       _logger->debug(WMLogLambda(log << "stopped via SIGTERM"));
                                   });

#ifdef SIGINT_WORKAROUND
        std::unique_ptr<app::os::InterProcessMessageQueue> ipcMessageQueue = nullptr;
        bool isSigintWorkaround = _programOptions->isSigintWorkaroundSet();
        if (isSigintWorkaround)
        {
            ipcMessageQueue = std::make_unique<app::os::InterProcessMessageQueue>();
        }
        _logger->debug(WMLogLambda(log << "using win32 sigterm ipc workaround"));
#endif
        visitVisitors(BeginLoop, 0);
        while (state > Stopped)
        {
            auto elapsed = _midiPlayerImpl.elapsed();
            std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_THREAD_SLEEPTIME));
#ifdef SIGINT_WORKAROUND
            if (ipcMessageQueue && ipcMessageQueue->sigintReceived())
            {
                state = Stopped;
                _midiPlayerImpl.panic();
                _logger->debug(WMLogLambda(log << "stopped via win32 sigterm ipc workaround"));
            }
#endif
            if (elapsed >= end)
            {
                if (!loop)
                {
                    break;
                }
                _midiPlayerImpl.play(begin);
            }
            visitVisitors(Loop, elapsed);
        }
        visitVisitors(EndLoop, 0);
        _midiPlayerImpl.stop();
        _midiPlayerImpl.panic();
        _midiPlayerImpl.Backend::tearDown();
    }

    void MidiPlayer::visitVisitors(VisitorMessage msg, com::Ticks elapsed)
    {
        com::Ticks renderRangeBegin = _programOptions->isBeginSet() ? (_programOptions->getBegin() * com::PPQ) : 0;
        for (auto visitor : _loopVisitors.container)
        {
            switch (msg)
            {
            case BeginLoop:
                visitor->loopBegin();
                break;
            case EndLoop:
                visitor->loopEnd();
                break;
            case Loop:
                visitor->visit(elapsed + renderRangeBegin);
            }
        }
    }

    com::Ticks MidiPlayer::stop()
    {
        if (state == Stopped)
        {
            return 0;
        }
        auto position = _midiPlayerImpl.elapsed();
        _midiPlayerImpl.stop();
        state = Stopped;
        return position;
    }

    void MidiPlayer::initFluidSynthInstances()
    {
        const auto &deviceConfigs = com::getConfigServer().getDevices();
        for (const auto &idConfPair : deviceConfigs)
        {
            const auto &conf = idConfPair.second;
            if (conf.type != com::DeviceConfig::FluidSynth)
            {
                continue;
            }
            const auto &soundFontFile = conf.deviceId;
            FluidSynthBackend::createInstance(conf.deviceId, soundFontFile);
        }
    }
}