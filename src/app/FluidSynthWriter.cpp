

#include "FluidSynthWriter.h"
#include <fstream>
#include <com/config/configServer.h>

namespace 
{
    const double FLUID_SYNTH_SEQUENCER_TIMESCALE = 1000;
    const double FLUID_SYNTH_DEFAULT_GAIN = 1.0;
    const long double FLUID_SYNTH_HEADROOM_MILLIS = 500;
}

namespace app
{
    void FluidSynthWriter::initSynth(const std::string &soundFondPath)
    {
        std::lock_guard<Mutex> lock(_renderMutex);
        initLibraryFunctions();
        settings = _new_fluid_settings();
        _fluid_settings_setstr(settings, "audio.driver", "file");
        _fluid_settings_setnum(settings, "synth.gain", FLUID_SYNTH_DEFAULT_GAIN);
        _fluid_settings_setnum(settings, "synth.sample-rate", sampleRate());
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
        std::lock_guard<Mutex> lock(_renderMutex);
        Base::tearDownSynth();
    }

    FluidSynthWriter::SoundFontId FluidSynthWriter::addSoundFont(const std::string &soundFondPath)
    {
        std::lock_guard<Mutex> lock(_renderMutex);
        if (soundFondPath.empty())
        {
            return FLUID_FAILED;
        }
        auto sfont_id = _fluid_synth_sfload(synth, soundFondPath.c_str(), 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw std::runtime_error("Loading the SoundFont " + soundFondPath + " failed!");
        }
        return sfont_id;
    }

    std::string FluidSynthWriter::findFluidSynthLibraryPath() const
    {
        _logger->babble(WMLogLambda(log << "findFluidSynthLibraryPath: '" << _libPath << "'"));
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
                else 
                {
                    const auto &devices = com::getConfigServer().getDevices();
                    auto deviceIt = devices.find(deviceId);
                    if (deviceIt == devices.end())
                    {
                        return;
                    }
                    sfId = addSoundFont(deviceIt->second.deviceId);
                    soundFontIds.insert(std::make_pair(deviceId, sfId));
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
        }
    }

    bool FluidSynthWriter::addEvent(const com::midi::Event& event)
    {
        std::lock_guard<Mutex> lock(_renderMutex);
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

    void FluidSynthWriter::render(int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr)
    {
        std::lock_guard<Mutex> lock(_renderMutex);
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
}