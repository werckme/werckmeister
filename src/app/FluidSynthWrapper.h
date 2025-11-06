#pragma once

#include <memory>
#include <vector>
#include <fluidsynth.h>
#include <boost/dll.hpp>
#include <com/midi.hpp>


// https://www.fluidsynth.org/api-1.x/synth_8h.html

#if FLUIDSYNTH_VERSION_MAJOR < 2
#error "Fluidsynth version >2 required"
#endif

#define new_fluid_settings_ftype fluid_settings_t *()
#define new_fluid_synth_ftype fluid_synth_t *(fluid_settings_t *)
#define new_fluid_audio_driver_ftype fluid_audio_driver_t *(fluid_settings_t *, fluid_synth_t *)
#define fluid_synth_sfload_ftype int(fluid_synth_t *, const char *, int)
#define fluid_synth_noteon_ftype int(fluid_synth_t *, int, int, int)
#define fluid_synth_noteoff_ftype int(fluid_synth_t *, int, int)
#define fluid_synth_program_change_ftype int(fluid_synth_t *, int, int)
#define fluid_synth_cc_ftype int(fluid_synth_t *, int, int, int)
#define fluid_synth_pitch_bend_ftype int(fluid_synth_t *, int, int)
#define fluid_settings_setstr_ftype int(fluid_settings_t *, const char *, const char *)
#define fluid_settings_setint_ftype int(fluid_settings_t *, const char *, int)
#define fluid_settings_setnum_ftype int(fluid_settings_t *, const char *, double)
#define fluid_audio_driver_register_ftype int(const char **)
#define delete_fluid_audio_driver_ftype void(fluid_audio_driver_t *)
#define delete_fluid_synth_ftype void(fluid_synth_t *)
#define delete_fluid_settings_ftype void(fluid_settings_t *)
#define new_fluid_sequencer2_ftype fluid_sequencer_t*(int) 	
#define fluid_sequencer_register_fluidsynth_ftype fluid_seq_id_t(fluid_sequencer_t *, fluid_synth_t *)
#define fluid_sequencer_unregister_client_ftype void(fluid_sequencer_t *, fluid_seq_id_t)
#define fluid_sequencer_register_client_ftype void(fluid_sequencer_t*, const char *name, fluid_event_callback_t, void*)
#define delete_fluid_sequencer_ftype void(fluid_sequencer_t*)
#define new_fluid_event_ftype fluid_event_t*()
#define fluid_event_set_source_ftype void(fluid_event_t*, fluid_seq_id_t)
#define fluid_event_set_dest_ftype void(fluid_event_t*, fluid_seq_id_t)
#define fluid_sequencer_send_at_ftype int(fluid_sequencer_t*, fluid_event_t*, unsigned int, int)
#define delete_fluid_event_ftype void(fluid_event_t*)
#define fluid_sequencer_get_tick_ftype unsigned int(fluid_sequencer_t*)
#define fluid_event_noteon_ftype void(fluid_event_t*, int, short, short)
#define fluid_event_noteoff_ftype void(fluid_event_t*, int, short)
#define fluid_event_control_change_ftype void(fluid_event_t*, int, short, int)
#define fluid_event_program_change_ftype void(fluid_event_t*, int, int)
#define fluid_event_pitch_bend_ftype void(fluid_event_t*, int, int)
#define fluid_event_channel_pressure_ftype void(fluid_event_t*, int, int)
#define fluid_event_key_pressure_ftype void(fluid_event_t*, int, short, int)
#define fluid_sequencer_set_time_scale_ftype void(fluid_sequencer_t*, double)
#define fluid_synth_write_float_ftype int(fluid_synth_t *, int, void*, int, int, void*, int, int)

namespace app
{
	class FluidSynth
	{
	public:
		FluidSynth();
		FluidSynth(const std::string &soundfontPath);
		FluidSynth(const FluidSynth &&) = delete;
		FluidSynth &operator=(const FluidSynth &&) = delete;
		virtual ~FluidSynth();
		void send(const com::midi::Event &event, long double elapsedMillis);
		void seek(long double millis);
	protected:
		virtual void initSynth(const std::string soundFondPath);
		virtual void tearDownSynth();
		void initLibraryFunctions();
		virtual std::string findFluidSynthLibraryPath() const;
		std::function<new_fluid_settings_ftype> _new_fluid_settings;
		std::function<new_fluid_synth_ftype> _new_fluid_synth;
		std::function<new_fluid_audio_driver_ftype> _new_fluid_audio_driver;
		std::function<fluid_synth_sfload_ftype> _fluid_synth_sfload;
		std::function<fluid_synth_noteon_ftype> _fluid_synth_noteon;
		std::function<fluid_synth_noteoff_ftype> _fluid_synth_noteoff;
		std::function<fluid_synth_program_change_ftype> _fluid_synth_program_change;
		std::function<fluid_synth_cc_ftype> _fluid_synth_cc;
		std::function<fluid_synth_pitch_bend_ftype> _fluid_synth_pitch_bend;
		std::function<fluid_settings_setstr_ftype> _fluid_settings_setstr;
		std::function<fluid_settings_setint_ftype> _fluid_settings_setint;
		std::function<fluid_settings_setnum_ftype> _fluid_settings_setnum;
		std::function<fluid_audio_driver_register_ftype> _fluid_audio_driver_register;
		std::function<delete_fluid_audio_driver_ftype> _delete_fluid_audio_driver;
		std::function<delete_fluid_synth_ftype> _delete_fluid_synth;
		std::function<delete_fluid_settings_ftype> _delete_fluid_settings;
		std::function<new_fluid_sequencer2_ftype> _new_fluid_sequencer2;
		std::function<fluid_sequencer_register_fluidsynth_ftype> _fluid_sequencer_register_fluidsynth;
		std::function<fluid_sequencer_register_client_ftype> _fluid_sequencer_register_client;
		std::function<delete_fluid_sequencer_ftype> _delete_fluid_sequencer;
		std::function<new_fluid_event_ftype> _new_fluid_event;
		std::function<fluid_event_set_source_ftype> _fluid_event_set_source;
		std::function<fluid_event_set_dest_ftype> _fluid_event_set_dest;
		std::function<fluid_sequencer_send_at_ftype> _fluid_sequencer_send_at;
		std::function<delete_fluid_event_ftype> _delete_fluid_event;
		std::function<fluid_event_noteon_ftype> _fluid_event_noteon;
		std::function<fluid_event_noteoff_ftype> _fluid_event_noteoff;
		std::function<fluid_sequencer_get_tick_ftype> _fluid_sequencer_get_tick;
		std::function<fluid_event_control_change_ftype> _fluid_event_control_change;
		std::function<fluid_event_program_change_ftype> _fluid_event_program_change;
		std::function<fluid_event_pitch_bend_ftype> _fluid_event_pitch_bend;
		std::function<fluid_event_channel_pressure_ftype> _fluid_event_channel_pressure;
		std::function<fluid_event_key_pressure_ftype> _fluid_event_key_pressure;
		std::function<fluid_sequencer_unregister_client_ftype> _fluid_sequencer_unregister_client;
		std::function<fluid_sequencer_set_time_scale_ftype> _fluid_sequencer_set_time_scale;
		/*
			Useful for storing interleaved stereo (lout = rout, loff = 0, roff = 1, lincr = 2, rincr = 2).
		*/
		std::function<fluid_synth_write_float_ftype> _fluid_synth_write_float;

		boost::dll::shared_library *_library = nullptr;
		fluid_settings_t *settings = nullptr;
		fluid_synth_t *synth = nullptr;
		fluid_audio_driver_t *adriver = nullptr;
		fluid_sequencer_t* seq = nullptr;
		fluid_seq_id_t synthSeqID = 0;
		bool midiEventToFluidEvent(const com::midi::Event& src, fluid_event_t& evt, bool doThrow = true);
		long double _playerOffsetMillis = 0;
	};
	typedef std::shared_ptr<FluidSynth> FluidSynthPtr;
}
