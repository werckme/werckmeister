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
        auto libPath                = findFluidSynthLibraryPath();
        _library                    = new boost::dll::shared_library(libPath); 
        new_fluid_settings          = _library->get<new_fluid_settings_ftype>("new_fluid_settings");
        new_fluid_synth             = _library->get<new_fluid_synth_ftype>("new_fluid_synth");
        new_fluid_audio_driver      = _library->get<new_fluid_audio_driver_ftype>("new_fluid_audio_driver");
        fluid_synth_sfload          = _library->get<fluid_synth_sfload_ftype>("fluid_synth_sfload");
        fluid_synth_noteon          = _library->get<fluid_synth_noteon_ftype>("fluid_synth_noteon");
        fluid_synth_noteoff         = _library->get<fluid_synth_noteoff_ftype>("fluid_synth_noteoff");
        fluid_synth_program_change  = _library->get<fluid_synth_program_change_ftype>("fluid_synth_program_change");
        fluid_settings_setstr       = _library->get<fluid_settings_setstr_ftype>("fluid_settings_setstr");
        fluid_audio_driver_register = _library->get<fluid_audio_driver_register_ftype>("fluid_audio_driver_register");
        delete_fluid_audio_driver   = _library->get<delete_fluid_audio_driver_ftype>("delete_fluid_audio_driver");
        delete_fluid_synth          = _library->get<delete_fluid_synth_ftype>("delete_fluid_synth");
        delete_fluid_settings       = _library->get<delete_fluid_settings_ftype>("delete_fluid_settings");	
    }

    void FluidSynth::tearDownSynth()
    {
        if (adriver) {
            delete_fluid_audio_driver(adriver);
            adriver = nullptr;
        }
        if (synth) {
            delete_fluid_synth(synth);
            synth = nullptr;
        }
        if (settings) {
            delete_fluid_settings(settings);
            settings = nullptr;
        }
    }

    std::string FluidSynth::findFluidSynthLibraryPath() const
    {
        return "/usr/lib/libfluidsynth.so";
    }

    void FluidSynth::initSynth(const std::string soundFondPath)
    {
        const char *adrivers[2];
        adrivers[0] = "jack";
        adrivers[1] = NULL; /* NULL terminate the array */

        int res = fluid_audio_driver_register(adrivers);

        if(res != FLUID_OK)
        {
            throw new std::runtime_error("adriver reg err");
        }

        /* Create the settings. */
        settings = new_fluid_settings();

        res = fluid_settings_setstr(settings, "audio.driver", adrivers[0]);

        if(res != FLUID_OK)
        {
            throw new std::runtime_error("audio.driver set err");
        }	

        /* Change the settings if necessary*/
        /* Create the synthesizer. */
        synth = new_fluid_synth(settings);
        /* Create the audio driver. The synthesizer starts playing as soon
        as the driver is created. */
        adriver = new_fluid_audio_driver(settings, synth);
        /* Load a SoundFont and reset presets (so that new instruments
        * get used from the SoundFont) */
        auto sfont_id = fluid_synth_sfload(synth, "/usr/share/soundfonts/FluidR3_GM.sf2", 1);
        if (sfont_id == FLUID_FAILED)
        {
            throw new std::runtime_error("Loading the SoundFont failed!");
        }
    }
}