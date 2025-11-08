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
    }

    FluidSynth::FluidSynth(const std::string &soundfontPath) : FluidSynth()
    {
        initLibraryFunctions();
        initSynth(soundfontPath);
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
        if (adriver)
        {
            _delete_fluid_audio_driver(adriver);
            adriver = nullptr;
        }
        if (seq) 
        {
            _fluid_sequencer_unregister_client(seq, synthSeqID);
            _delete_fluid_sequencer(seq);
            seq = nullptr;
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

    void FluidSynth::initSynth(const std::string soundFondPath)
    {
        settings = _new_fluid_settings();

        _fluid_settings_setnum(settings, "synth.gain", FLUID_SYNTH_DEFAULT_GAIN);
        synth = _new_fluid_synth(settings);
        adriver = _new_fluid_audio_driver(settings, synth);
        seq = _new_fluid_sequencer2(0);
        _fluid_sequencer_set_time_scale(seq, FLUID_SYNTH_SEQUENCER_TIMESCALE);
        synthSeqID = _fluid_sequencer_register_fluidsynth(seq, synth);
        if (synthSeqID == FLUID_FAILED)
        {
            throw std::runtime_error("error initializing fluidsynth sequencer");
        }
        auto sfont_id = _fluid_synth_sfload(synth, soundFondPath.c_str(), 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw std::runtime_error("Loading the SoundFont " + soundFondPath + "  failed!");
        }
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
}
