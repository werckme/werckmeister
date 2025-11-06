

#include "FluidSynthWriter.h"


namespace 
{
    const double FLUID_SYNTH_SEQUENCER_TIMESCALE = 1000;
    const double FLUID_SYNTH_DEFAULT_GAIN = 1.0;
    const long double FLUID_SYNTH_HEADROOM_MILLIS = 500;
}

namespace app
{
    void FluidSynthWriter::initSynth(const std::string soundFondPath)
    {
        initLibraryFunctions();
        settings = _new_fluid_settings();
        _fluid_settings_setstr(settings, "audio.driver", "file");
        _fluid_settings_setnum(settings, "synth.gain", FLUID_SYNTH_DEFAULT_GAIN);
        synth = _new_fluid_synth(settings);
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
            throw std::runtime_error("Loading the SoundFont " + soundFondPath + " failed!");
        }
    }

    std::string FluidSynthWriter::findFluidSynthLibraryPath() const
    {
        return "/lib/x86_64-linux-gnu/libfluidsynth.so.3"; // TODO
    }

    bool FluidSynthWriter::addEvent(const com::midi::Event& event)
    {
        fluid_event_t* fluid_event = _new_fluid_event();
        const bool doThrow = false;
        if (!midiEventToFluidEvent(event, *fluid_event, doThrow))
        {
            return false;
        }
        _fluid_event_set_dest(fluid_event, synthSeqID);
        long double pos =  60.0 * 1000.0 * event.absPosition() / (120.0 * com::PPQ) + FLUID_SYNTH_HEADROOM_MILLIS;
        auto sendResult = _fluid_sequencer_send_at(seq, fluid_event, (int)(pos), 1);
        if (sendResult == FLUID_FAILED)
        {
            throw std::runtime_error("sending fluid synth sequence event failed at time: " + std::to_string(pos) + "ms");
        }
        _delete_fluid_event(fluid_event);
        return true;
    }

}