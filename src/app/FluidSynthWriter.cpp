

#include "FluidSynthWriter.h"
#include <fstream>
#include <com/config/configServer.h>
#include <vector>
#include <string>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <com/werckmeister.hpp>

namespace 
{
    const double FLUID_SYNTH_SEQUENCER_TIMESCALE = 1000;
    const double FLUID_SYNTH_DEFAULT_GAIN = 1.0;
    const long double FLUID_SYNTH_HEADROOM_MILLIS = 0;
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
                _logger->babble(WMLogLambda(log << "adding device: " << deviceConfig.name << ", " << deviceConfig.deviceId));
                com::getConfigServer().addDevice(deviceConfig);

                auto sfId = addSoundFont(deviceConfig.deviceId);
                soundFontIds.insert(std::make_pair(deviceConfig.name, sfId));

            }
        }
        bool eventIsMsb = event.eventType() == com::midi::Controller && event.parameter1() == 0;
        bool eventIsProgramChange = event.eventType() == com::midi::ProgramChange || eventIsMsb;
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

    double FluidSynthWriter::getSongPositionSeconds() const
    {
        if (synth == nullptr || seq == nullptr)
        {
            return 0;
        }
        double ticks = _fluid_sequencer_get_tick(seq);
        return ticks / FLUID_SYNTH_SEQUENCER_TIMESCALE;
    }
}