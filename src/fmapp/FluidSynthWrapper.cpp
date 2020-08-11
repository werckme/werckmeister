#include "FluidSynthWrapper.h"
#include <boost/dll.hpp>

namespace fmapp {
    FluidSynth::FluidSynth(const std::string &soundfontPath)
    {
        initLibraryFunctions();
        initSynth(soundfontPath);
    }
    FluidSynth::~FluidSynth() 
    {
        if (_library) {
            tearDownSynth();
            delete _library;
            _library = nullptr;
        }
    }    
    void FluidSynth::initLibraryFunctions()
    {
        auto libPath                 = findFluidSynthLibraryPath();
        _library                     = new boost::dll::shared_library(libPath); 
        _new_fluid_settings          = _library->get<new_fluid_settings_ftype>("new_fluid_settings");
        _new_fluid_synth             = _library->get<new_fluid_synth_ftype>("new_fluid_synth");
        _new_fluid_audio_driver      = _library->get<new_fluid_audio_driver_ftype>("new_fluid_audio_driver");
        _fluid_synth_sfload          = _library->get<fluid_synth_sfload_ftype>("fluid_synth_sfload");
        _fluid_synth_noteon          = _library->get<fluid_synth_noteon_ftype>("fluid_synth_noteon");
        _fluid_synth_noteoff         = _library->get<fluid_synth_noteoff_ftype>("fluid_synth_noteoff");
        _fluid_synth_program_change  = _library->get<fluid_synth_program_change_ftype>("fluid_synth_program_change");
        _fluid_settings_setstr       = _library->get<fluid_settings_setstr_ftype>("fluid_settings_setstr");
        _fluid_audio_driver_register = _library->get<fluid_audio_driver_register_ftype>("fluid_audio_driver_register");
        _delete_fluid_audio_driver   = _library->get<delete_fluid_audio_driver_ftype>("delete_fluid_audio_driver");
        _delete_fluid_synth          = _library->get<delete_fluid_synth_ftype>("delete_fluid_synth");
        _delete_fluid_settings       = _library->get<delete_fluid_settings_ftype>("delete_fluid_settings");	
    }

    void FluidSynth::tearDownSynth()
    {
        if (adriver) {
            _delete_fluid_audio_driver(adriver);
            adriver = nullptr;
        }
        if (synth) {
            _delete_fluid_synth(synth);
            synth = nullptr;
        }
        if (settings) {
            _delete_fluid_settings(settings);
            settings = nullptr;
        }
    }

    std::string FluidSynth::findFluidSynthLibraryPath() const
    {
        return "libfluidsynth-2.dll";
        // return "/usr/lib/libfluidsynth.so";
    }

    void FluidSynth::initSynth(const std::string soundFondPath)
    {
        settings = _new_fluid_settings();
        synth = _new_fluid_synth(settings);
        adriver = _new_fluid_audio_driver(settings, synth);
        auto sfont_id = _fluid_synth_sfload(synth, soundFondPath.c_str(), 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw new std::runtime_error("Loading the SoundFont failed!");
        }
    }

    void FluidSynth::send(const fm::midi::Event& event)
    {
        switch (event.eventType()) {
        case fm::midi::NoteOn:
            _fluid_synth_noteon(synth, event.channel(), event.parameter1(), event.parameter2());
            break;
        case fm::midi::NoteOff:
            _fluid_synth_noteoff(synth, event.channel(), event.parameter1());
            break;
        case fm::midi::ProgramChange:
            _fluid_synth_program_change(synth, event.channel(), event.parameter1());
            break;
        case fm::midi::Controller:
        default: break;
        }

    }
}