#include "FluidSynthWrapper.h"
#include <boost/dll.hpp>
#include <com/werckmeister.hpp>
#include <com/config.hpp>
#include <iostream>

namespace 
{
    const double FLUID_SYNTH_SEQUENCER_TIMESCALE = 1000;
    const double FLUID_SYNTH_DEFAULT_GAIN = 1.0;
    const long double FLUID_SYNTH_HEADROOM_MILLIS = 500;
    std::unique_ptr<boost::dll::shared_library> _library;
}

namespace app
{

    FluidSynth::FluidSynth()
    {
        initLibraryFunctions();
        initSynth();
    }

    FluidSynth::~FluidSynth()
    {
        tearDownSynth();
    }

    bool FluidSynth::midiEventToFluidEvent(const com::midi::Event& src, fluid_event_t& evt, bool doThrow)
    {
        int chan = (int)src.channel();

        switch (src.eventType())
        {
        case com::midi::NoteOff:
            _fluid_event_noteoff(&evt, chan, (short)src.parameter1());
            break;

        case com::midi::NoteOn:
            _fluid_event_noteon(&evt, chan, (short)src.parameter1(), (short)src.parameter2());
            break;

        case com::midi::Controller:
            _fluid_event_control_change(&evt, chan, (short)src.parameter1(), (short)src.parameter2());
            break;

        case com::midi::ProgramChange:
            _fluid_event_program_change(&evt, chan, (short)src.parameter1());
            break;

        case com::midi::PitchBend:
            _fluid_event_pitch_bend(&evt, chan, (int)(src.pitchBend() * (double)com::midi::MaxPitchbend));
            break;

        case com::midi::ChannelAftertouch:
            _fluid_event_channel_pressure(&evt, chan, (short)src.parameter1());
            break;

        case com::midi::NoteAftertouch:
            _fluid_event_key_pressure(&evt, chan, (short)src.parameter1(), (short)src.parameter2());
            break;

        default:
            if (!doThrow)
            {
                return false;
            }
            throw com::Exception("fluidsynth: midi event type not implemented");
        }
        return true;
    }

    void FluidSynth::initLibraryFunctions()
    {
        auto libPath = findFluidSynthLibraryPath();
        try
        {
            if (!_library)
            {
                _library = std::make_unique<boost::dll::shared_library>(libPath);
            }
            _new_fluid_settings = _library->get<new_fluid_settings_ftype>("new_fluid_settings");
            _new_fluid_synth = _library->get<new_fluid_synth_ftype>("new_fluid_synth");
            _new_fluid_audio_driver = _library->get<new_fluid_audio_driver_ftype>("new_fluid_audio_driver");
            _fluid_synth_sfload = _library->get<fluid_synth_sfload_ftype>("fluid_synth_sfload");
            _fluid_synth_noteon = _library->get<fluid_synth_noteon_ftype>("fluid_synth_noteon");
            _fluid_synth_noteoff = _library->get<fluid_synth_noteoff_ftype>("fluid_synth_noteoff");
            _fluid_synth_program_change = _library->get<fluid_synth_program_change_ftype>("fluid_synth_program_change");
            _fluid_synth_cc = _library->get<fluid_synth_cc_ftype>("fluid_synth_cc");
            _fluid_synth_pitch_bend = _library->get<fluid_synth_pitch_bend_ftype>("fluid_synth_pitch_bend");
            _fluid_settings_setstr = _library->get<fluid_settings_setstr_ftype>("fluid_settings_setstr");
            _fluid_settings_setint = _library->get<fluid_settings_setint_ftype>("fluid_settings_setint");
            _fluid_settings_setnum = _library->get<fluid_settings_setnum_ftype>("fluid_settings_setnum");
            _fluid_audio_driver_register = _library->get<fluid_audio_driver_register_ftype>("fluid_audio_driver_register");
            _delete_fluid_audio_driver = _library->get<delete_fluid_audio_driver_ftype>("delete_fluid_audio_driver");
            _delete_fluid_synth = _library->get<delete_fluid_synth_ftype>("delete_fluid_synth");
            _delete_fluid_settings = _library->get<delete_fluid_settings_ftype>("delete_fluid_settings");
            _new_fluid_sequencer2 = _library->get<new_fluid_sequencer2_ftype>("new_fluid_sequencer2");
            _fluid_sequencer_register_fluidsynth = _library->get<fluid_sequencer_register_fluidsynth_ftype>("fluid_sequencer_register_fluidsynth");
            _fluid_sequencer_unregister_client = _library->get<fluid_sequencer_unregister_client_ftype>("fluid_sequencer_unregister_client");
            _fluid_sequencer_register_client = _library->get<fluid_sequencer_register_client_ftype>("fluid_sequencer_register_client");
            _delete_fluid_sequencer = _library->get<delete_fluid_sequencer_ftype>("delete_fluid_sequencer");
            _new_fluid_event = _library->get<new_fluid_event_ftype>("new_fluid_event");
            _fluid_event_set_source = _library->get<fluid_event_set_source_ftype>("fluid_event_set_source");
            _fluid_event_set_dest = _library->get<fluid_event_set_dest_ftype>("fluid_event_set_dest");
            _fluid_sequencer_send_at = _library->get<fluid_sequencer_send_at_ftype>("fluid_sequencer_send_at");
            _delete_fluid_event = _library->get<delete_fluid_event_ftype>("delete_fluid_event");
            _fluid_event_noteon = _library->get<fluid_event_noteon_ftype>("fluid_event_noteon");
            _fluid_event_noteoff = _library->get<fluid_event_noteoff_ftype>("fluid_event_noteoff");
            _fluid_sequencer_get_tick = _library->get<fluid_sequencer_get_tick_ftype>("fluid_sequencer_get_tick");
            _fluid_sequencer_get_tick = _library->get<fluid_sequencer_get_tick_ftype>("fluid_sequencer_get_tick");
            _fluid_event_control_change = _library->get<fluid_event_control_change_ftype>("fluid_event_control_change");
            _fluid_event_program_change = _library->get<fluid_event_program_change_ftype>("fluid_event_program_change");
            _fluid_event_pitch_bend = _library->get<fluid_event_pitch_bend_ftype>("fluid_event_pitch_bend");
            _fluid_event_channel_pressure = _library->get<fluid_event_channel_pressure_ftype>("fluid_event_channel_pressure");
            _fluid_event_key_pressure = _library->get<fluid_event_key_pressure_ftype>("fluid_event_key_pressure");
            _fluid_sequencer_set_time_scale = _library->get<fluid_sequencer_set_time_scale_ftype>("fluid_sequencer_set_time_scale");
            _fluid_synth_write_float = _library->get<fluid_synth_write_float_ftype>("fluid_synth_write_float");
            _fluid_synth_program_select = _library->get<fluid_synth_program_select_ftype>("fluid_synth_program_select");
            _new_fluid_file_renderer = _library->get<new_fluid_file_renderer_ftype>("fluid_synth_program_select");
            _fluid_file_renderer_process_block = _library->get<fluid_file_renderer_process_block_ftype>("fluid_synth_program_select");
            _delete_fluid_file_renderer = _library->get<delete_fluid_file_renderer_ftype>("fluid_synth_program_select");
            _new_fluid_player = _library->get<new_fluid_player_ftype>("new_fluid_player");
            _fluid_player_stop = _library->get<fluid_player_stop_ftype>("fluid_player_stop");
            _fluid_player_set_playback_callback = _library->get<fluid_player_set_playback_callback_ftype>("fluid_player_set_playback_callback");
            _fluid_player_seek = _library->get<fluid_player_seek_ftype>("fluid_player_seek");
            _fluid_player_play = _library->get<fluid_player_play_ftype>("fluid_player_play");
            _fluid_player_join = _library->get<fluid_player_join_ftype>("fluid_player_join");
            _fluid_player_get_current_tick = _library->get<fluid_player_get_current_tick_ftype>("fluid_player_get_current_tick");
            _fluid_player_add_mem = _library->get<fluid_player_add_mem_ftype>("fluid_player_add_mem");
            _delete_fluid_player = _library->get<delete_fluid_player_ftype>("delete_fluid_player");
            _fluid_player_get_midi_tempo = _library->get<fluid_player_get_midi_tempo_ftype>("fluid_player_get_midi_tempo");
            _fluid_player_get_division = _library->get<fluid_player_get_division_ftype>("fluid_player_get_division");
            _fluid_player_set_tick_callback = _library->get<fluid_player_set_tick_callback_ftype>("fluid_player_set_tick_callback");
        }
        catch (const std::exception &ex)
        {
            std::stringstream ss;
            ss << "loading library \"" + libPath + "\" failed: " << std::endl
               << ex.what();
            throw std::runtime_error(ss.str());
        }
        catch (...)
        {
            std::stringstream ss;
            ss << "loading library \"" + libPath + "\" failed: unkown error";
            throw std::runtime_error(ss.str());
        }
    }

    void FluidSynth::tearDownSynth()
    {
        if (seq) 
        {
            _fluid_sequencer_unregister_client(seq, synthSeqID);
            _delete_fluid_sequencer(seq);
            seq = nullptr;
        }
        if (adriver)
        {
            _delete_fluid_audio_driver(adriver);
            adriver = nullptr;
        }

        if (synth)
        {
            _delete_fluid_synth(synth);
            synth = nullptr;
        }
        if (settings)
        {
            _delete_fluid_settings(settings);
            settings = nullptr;
        }
    }

    std::string FluidSynth::findFluidSynthLibraryPath() const
    {
        auto &wm = com::getWerckmeister();
        std::string libraryPath(LIB_FLUIDSYNTH_FILENAME);
        const auto &searchPaths = com::LibfluidSynthSearchPaths();
        for (const auto &searchPath : searchPaths)
        {
            wm.addSearchPath(searchPath);
        }
        return wm.resolvePath(libraryPath);
    }

    void FluidSynth::initSynth()
    {
        settings = _new_fluid_settings();

        auto ok =_fluid_settings_setnum(settings, "synth.gain", FLUID_SYNTH_DEFAULT_GAIN);
        if (ok != FLUID_OK) {
            throw std::runtime_error("Setting synth.gain failed");
        }
        ok = _fluid_settings_setstr(settings, "synth.midi-bank-select", "mma");
        if (ok != FLUID_OK) {
            throw std::runtime_error("Setting synth.midi-bank-select failed");
        }

        synth = _new_fluid_synth(settings);
        adriver = _new_fluid_audio_driver(settings, synth);
        seq = _new_fluid_sequencer2(0);
        _fluid_sequencer_set_time_scale(seq, FLUID_SYNTH_SEQUENCER_TIMESCALE);
        synthSeqID = _fluid_sequencer_register_fluidsynth(seq, synth);
        if (synthSeqID == FLUID_FAILED)
        {
            throw std::runtime_error("error initializing fluidsynth sequencer");
        }
    }

    FluidSynth::SoundFontId FluidSynth::addSoundFont(const DeviceId& deviceId, const std::string &soundFontPath)
    {
        if (soundFontIdMap.find(deviceId) != soundFontIdMap.end())
		{
			throw std::runtime_error("device '" + deviceId + "' already added");
		}
        auto absPath = com::getWerckmeister().resolvePath(soundFontPath);
        auto sfont_id = _fluid_synth_sfload(synth, absPath.c_str(), 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw std::runtime_error("Loading the SoundFont " + absPath + " failed!");
        }
        soundFontIdMap.insert({deviceId, sfont_id});
        return sfont_id;
    }

    void FluidSynth::setPreset(SoundFontId sfId, int channel, int presetNr)
    {
        auto bank = msbPerChannel[channel] * 128 + lsbPerChannel[channel];
        if (_fluid_synth_program_select(synth, channel, sfId, bank, presetNr) != FLUID_OK)
        {
            throw std::runtime_error("fluid_synth_program_select failed");
        }
    }

    void FluidSynth::setMsb(int channel, int msb)
    {
        if (channel < 0 || channel > 15)
        {
            throw std::runtime_error("invalid channel " + std::to_string(channel));
        }
        msbPerChannel[channel] = msb;
    }

    void FluidSynth::setLsb(int channel, int lsb)
    {
        if (channel < 0 || channel > 15)
        {
            throw std::runtime_error("invalid channel " + std::to_string(channel));
        }
        lsbPerChannel[channel] = lsb;
    }

    void FluidSynth::send(const com::midi::Event &event, long double elapsedMillis)
    {
        fluid_event_t* fluid_event = _new_fluid_event();
        midiEventToFluidEvent(event, *fluid_event);
        _fluid_event_set_dest(fluid_event, synthSeqID);
        long double playPos = elapsedMillis + _playerOffsetMillis + FLUID_SYNTH_HEADROOM_MILLIS;
        auto sendResult = _fluid_sequencer_send_at(seq, fluid_event, (int)(playPos), 1);
        if (sendResult == FLUID_FAILED)
        {
            throw std::runtime_error("sending fluid synth sequence event failed at time: " + std::to_string(elapsedMillis) + "ms");
        }
        _delete_fluid_event(fluid_event);
    }

    void FluidSynth::seek(long double millis)
    {
        _playerOffsetMillis = -millis;
    }

    void FluidSynth::setCC(int ch, int cc, int value)
    {
        _fluid_synth_cc(synth, ch, cc, value);
    }

    void FluidSynth::handleMetaEvent(const com::midi::Event& event)
    {
        if (event.metaEventType() == com::midi::CustomMetaEvent)
        {
            auto customEvent = com::midi::Event::MetaGetCustomData(event.metaData(), event.metaDataSize());
            if (customEvent.type == com::midi::CustomMetaData::SetDevice)
            {
                std::string deviceId(customEvent.data.begin(), customEvent.data.end());
                auto soundFontIdsIt = soundFontIdMap.find(deviceId);
                SoundFontId sfId = FLUID_FAILED;
                if (soundFontIdsIt != soundFontIdMap.end())
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
        }
    }
}
