

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
        _fluid_settings_setnum(settings, "synth.sample-rate", sampleRate());
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
        if (_libPath.empty())
        {
            return Base::findFluidSynthLibraryPath();
        }
        return _libPath;
    }

    bool FluidSynthWriter::addEvent(const com::midi::Event& event)
    {
		if (event.eventType() == com::midi::MetaEvent && event.metaEventType() == com::midi::Tempo)
		{
			auto metaIntValue = com::midi::Event::MetaGetIntValue(event.metaData(), event.metaDataSize());
			_tempo = com::midi::MicrosecondsPerMinute / (double)metaIntValue;
		}
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
        auto result = _fluid_synth_write_float(synth, len, lout, loff, lincr, rout, roff, rincr);
        if (result != FLUID_OK)
        {
            throw std::runtime_error("fluid_synth_nwrite_float failed.");
        }
    }
}