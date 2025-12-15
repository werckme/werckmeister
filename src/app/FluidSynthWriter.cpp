

#include "FluidSynthWriter.h"
#include <fstream>
#include <com/config/configServer.h>
#include <vector>
#include <string>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <com/werckmeister.hpp>
#include <com/config.hpp>

namespace 
{
    const double FLUID_SYNTH_SEQUENCER_TIMESCALE = 1000;
    const double FLUID_SYNTH_DEFAULT_GAIN = 1.0;
    const long double FLUID_SYNTH_HEADROOM_MILLIS = 0;
    int OnTickEventHandler(void *data, int tick)
    {
        reinterpret_cast<app::FluidSynthWriter*>(data)->onTickEventCallback(tick);
        return FLUID_OK;
    }
}

namespace app
{
    void FluidSynthWriter::initSynth(const std::string &soundFondPath)
    {
        _logger->babble(WMLogLambda(log << "FluidSynthWriter:: init fluidsynth: " << soundFondPath));
        initLibraryFunctions();
        settings = _new_fluid_settings();
        _fluid_settings_setstr(settings, "audio.driver", "file");
        _fluid_settings_setnum(settings, "synth.gain", FLUID_SYNTH_DEFAULT_GAIN);
        _fluid_settings_setnum(settings, "synth.sample-rate", sampleRate());
        _fluid_settings_setstr(settings, "synth.midi-bank-select", "mma");
        synth = _new_fluid_synth(settings);
        seq = _new_fluid_sequencer2(0);
        _fluid_sequencer_set_time_scale(seq, FLUID_SYNTH_SEQUENCER_TIMESCALE);
        synthSeqID = _fluid_sequencer_register_fluidsynth(seq, synth);
        if (synthSeqID == FLUID_FAILED)
        {
            throw std::runtime_error("error initializing fluidsynth sequencer");
        }
    }

    void FluidSynthWriter::tearDownSynth()
    {
        _logger->babble(WMLogLambda(log << "FluidSynthWriter:: tear down fluidsynth"));
        if (player)
        {
             _fluid_player_stop(player);
             _fluid_player_join(player);
             _delete_fluid_player(player);
             player = nullptr;
        }
        Base::tearDownSynth();
    }

    FluidSynthWriter::SoundFontId FluidSynthWriter::addSoundFont(const std::string &soundFondPath)
    {
        if (soundFondPath.empty())
        {
            return FLUID_FAILED;
        }
        auto absPath = com::getWerckmeister().resolvePath(soundFondPath);
        _logger->babble(WMLogLambda(log << "FluidSynthWriter:: addSoundFont: " << absPath));
        auto sfont_id = _fluid_synth_sfload(synth, absPath.c_str(), 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw std::runtime_error("Loading the SoundFont " + absPath + " failed!");
        }
        return sfont_id;
    }

    std::string FluidSynthWriter::findFluidSynthLibraryPath() const
    {
        _logger->babble(WMLogLambda(log << "FluidSynthWriter:: findFluidSynthLibraryPath: '" << _libPath << "'"));
        if (_libPath.empty())
        {
            return Base::findFluidSynthLibraryPath();
        }
        return _libPath;
    }

    void FluidSynthWriter::handleMetaEvent(const com::midi::Event& event)
    {
        if (event.eventType() == com::midi::MetaEvent && event.metaEventType() == com::midi::Tempo)
        {
            auto metaIntValue = com::midi::Event::MetaGetIntValue(event.metaData(), event.metaDataSize());
            _tempo = com::midi::MicrosecondsPerMinute / (double)metaIntValue;
        }
        if (event.metaEventType() == com::midi::CustomMetaEvent)
        {
            auto customEvent = com::midi::Event::MetaGetCustomData(event.metaData(), event.metaDataSize());
            if (customEvent.type == com::midi::CustomMetaData::SetDevice)
            {
                std::string deviceId(customEvent.data.begin(), customEvent.data.end());
                auto soundFontIdsIt = soundFontIds.find(deviceId);
                SoundFontId sfId = FLUID_FAILED;
                if (soundFontIdsIt != soundFontIds.end())
                {
                    sfId = soundFontIdsIt->second;
                } 
                if (sfId == FLUID_FAILED)
                {
                    return;
                }
                if (_fluid_synth_program_select(synth, event.channel(), sfId, 0, 0) != FLUID_OK)
                {
                    throw std::runtime_error("fluid_synth_program_select failed");
                }
            }
            if(customEvent.type == com::midi::CustomMetaData::DefineDevice)
            {
                std::string deviceIdWithSoundFont(customEvent.data.begin(), customEvent.data.end());
                std::vector<std::string> defParts;
                defParts.reserve(2);
                boost::split_regex(defParts, deviceIdWithSoundFont, boost::regex("://"));
                if (defParts.size() != 2)
                {
                    return;
                }
                com::DeviceConfig deviceConfig;
                deviceConfig.name = defParts[0];
                deviceConfig.deviceId = defParts[1];
                deviceConfig.type = com::DeviceConfig::FluidSynth;
                if (soundFontIds.find(deviceConfig.name) != soundFontIds.end())
                {
                    _logger->babble(WMLogLambda(log << "device: " << deviceConfig.name << "already added"));
                    return;
                }
                _logger->babble(WMLogLambda(log << "adding device: " << deviceConfig.name << ", " << deviceConfig.deviceId));
                com::getConfigServer().addDevice(deviceConfig);
                auto sfId = addSoundFont(deviceConfig.deviceId);
                soundFontIds.insert(std::make_pair(deviceConfig.name, sfId));
            }
        }
    }

    void FluidSynthWriter::onTickEventCallback(int tick)
    {
        if (_activeJumpPoint == nullptr)
        {
            return;
        }
        const auto &jumpPoint = *_activeJumpPoint;
        if (tick >= jumpPoint.fromPositionTicks) 
        {
            _fluid_player_seek(player, jumpPoint.toPositionTicks);
            if (_activeJumpPoint == &_tmpJumpoint)
            {
                _activeJumpPoint = nullptr;
            }
        }
    }

    void FluidSynthWriter::jump(const JumpPoint &jumpPoint)
    {
        _tmpJumpoint = jumpPoint;
        _activeJumpPoint = &_tmpJumpoint;
        _logger->babble(WMLogLambda(log << "jump: " << "f: " << _activeJumpPoint->fromPositionTicks << " t:" << _activeJumpPoint->toPositionTicks));
    }

    void FluidSynthWriter::setJumpPoints(const JumpPoints& jumpPoints)
    {
         _logger->babble(WMLogLambda(log << "setJumpPoints: " << jumpPoints.size()));
        _jumpPoints = jumpPoints;
        _activeJumpPoint = nullptr;
        updateJumpPointIndex();
    }

    void FluidSynthWriter::setJumpPoints(JumpPoints&& jumpPoints)
    {
        _logger->babble(WMLogLambda(log << "setJumpPoints: " << jumpPoints.size()));
        _jumpPoints = std::move(jumpPoints);
        _activeJumpPoint = nullptr;
        updateJumpPointIndex();
    }

    void FluidSynthWriter::updateJumpPointIndex()
    {
        _jumpPointsIndex = JumpPointsIndex(_jumpPoints.size(), nullptr);
        for(const auto& cit : _jumpPoints)
        {
            auto index = cit.second.index;
            if (index < 0)
            {
                throw std::runtime_error("invalid jumpPointIndex: " + std::to_string(index));
            }
            _jumpPointsIndex[index] = &cit.second;
        }
    }

    void FluidSynthWriter::setActiveJumpPoint(int jumpPointIndex)
    {
        _logger->babble(WMLogLambda(log << "setActiveJumpPoint: " << jumpPointIndex));
        if (jumpPointIndex == UndefinedJumpPointIndex)
        {
            _activeJumpPoint = nullptr;
            return;
        }
        if (jumpPointIndex < 0 || jumpPointIndex >= (int)_jumpPoints.size())
        {
            throw std::invalid_argument("invalid jump point: " 
                + std::to_string(jumpPointIndex)
                + " with a size of "
                + std::to_string(_jumpPoints.size())
            );
        }
        _activeJumpPoint = this->_jumpPointsIndex[jumpPointIndex];
         _logger->babble(WMLogLambda(log << "setActiveJumpPoint: " 
            << "f: " << _activeJumpPoint->fromPositionTicks
            << " t:" << _activeJumpPoint->toPositionTicks
            << " i:" << _activeJumpPoint->index));
    }

    bool FluidSynthWriter::addEvent(const com::midi::Event& event)
    {
        if (!seq)
        {
            return true;
        }
        handleMetaEvent(event);
        fluid_event_t* fluid_event = _new_fluid_event();
        const bool doThrow = false;
        if (!midiEventToFluidEvent(event, *fluid_event, doThrow))
        {
            return false;
        }
        _fluid_event_set_dest(fluid_event, synthSeqID);
        long double pos =  60.0 * 1000.0 * event.absPosition() / (_tempo * com::PPQ) + FLUID_SYNTH_HEADROOM_MILLIS;
        auto sendResult = _fluid_sequencer_send_at(seq, fluid_event, (int)(pos), 1);
        if (sendResult == FLUID_FAILED)
        {
            throw std::runtime_error("sending fluid synth sequence event failed at time: " + std::to_string(pos) + "ms");
        }
        _delete_fluid_event(fluid_event);
        return true;
    }

    void FluidSynthWriter::setMidiFileData(const unsigned char* data, size_t length)
    {
        player = _new_fluid_player(synth);
        _fluid_player_add_mem(player, data, length);
        _fluid_player_set_tick_callback(player, OnTickEventHandler, this);
        _fluid_player_play(player);
    }

    void FluidSynthWriter::render(int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr)
    {
        if (synth == nullptr)
        {
            return;
        }
        auto result = _fluid_synth_write_float(synth, len, lout, loff, lincr, rout, roff, rincr);
        if (result != FLUID_OK)
        {
            throw std::runtime_error("fluid_synth_nwrite_float failed.");
        }
    }

    void FluidSynthWriter::stop()
    {
        if (synth == nullptr)
        {
            return;
        }
        if (player == nullptr)
        {
            return;
        }
        _fluid_player_stop(player);
    }

    void FluidSynthWriter::play()
    {
        if (synth == nullptr)
        {
            return;
        }
        if (player == nullptr)
        {
            return;
        }
        _fluid_player_play(player);
    }

    void FluidSynthWriter::renderToFile(const std::string &outputPath, double seconds)
    {
        const int blockSize = 1024;
        _fluid_settings_setint(settings, "audio.period-size", blockSize);
        _fluid_settings_setstr(settings, "audio.file.name", outputPath.c_str());
        _fluid_settings_setstr(settings, "audio.file.format", "wav");
        int totalSamples = blockSize * 100; //sampleRate() * seconds;
        auto renderer = _new_fluid_file_renderer(synth);
        while (totalSamples > 0) 
        {
            _fluid_file_renderer_process_block(renderer);
            totalSamples -= blockSize;
        }
        _delete_fluid_file_renderer(renderer);
    }

    void FluidSynthWriter::setSongPositionSeconds(double songPosSeconds)
    {
        if (player == nullptr)
        {
            _logger->error(WMLogLambda(log << "seek is not supported when fluid synth midi player is not in use"));
            return;
        }
        int division = _fluid_player_get_division(player);
        int tempo_us = _fluid_player_get_midi_tempo(player);
        int ticks = (int)(songPosSeconds * division * 1e6 / tempo_us);
        _fluid_player_seek(player, ticks);
    }

    double FluidSynthWriter::getSongPositionSeconds() const
    {
        if (synth == nullptr)
        {
            return 0;
        }
        if (player != nullptr)
        {
            int ticks = _fluid_player_get_current_tick(player);
            int division = _fluid_player_get_division(player);
            int tempo_us = _fluid_player_get_midi_tempo(player);
            double seconds = ticks * (tempo_us / 1e6) / division;
            return seconds;
        }
        if (seq != nullptr)
        {
            double ticks = _fluid_sequencer_get_tick(seq);
            return ticks / FLUID_SYNTH_SEQUENCER_TIMESCALE;
        }
        return 0;
    }
}