#pragma once

#include <memory>
#include <vector>
#include <fluidsynth.h>
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
#define fluid_synth_program_select_ftype int(fluid_synth_t *, int, int, int, int)
#define new_fluid_file_renderer_ftype fluid_file_renderer_t*(fluid_synth_t*)
#define fluid_file_renderer_process_block_ftype int(fluid_file_renderer_t *)
#define delete_fluid_file_renderer_ftype void(fluid_file_renderer_t *)
#define new_fluid_player_ftype fluid_player_t*(fluid_synth_t * synth)
#define fluid_player_stop_ftype int(fluid_player_t * player)
#define fluid_player_set_playback_callback_ftype int(fluid_player_t * player, handle_midi_event_func_t handler, void * handler_data)
#define fluid_player_seek_ftype int(fluid_player_t * player, int ticks)
#define fluid_player_play_ftype int(fluid_player_t * player)
#define fluid_player_join_ftype int(fluid_player_t * player)
#define fluid_player_get_current_tick_ftype int(fluid_player_t * player)
#define fluid_player_add_mem_ftype int(fluid_player_t * player, const void * buffer, size_t len)
#define delete_fluid_player_ftype void(fluid_player_t * player)
#define fluid_player_get_midi_tempo_ftype int(fluid_player_t * player)
#define fluid_player_get_division_ftype int(fluid_player_t * player)
#define fluid_player_set_tick_callback_ftype int(fluid_player_t * player, handle_midi_tick_func_t handler, void * handler_data)

namespace app
{
	class FluidSynth
	{
	public:
		typedef int SoundFontId;
		typedef com::String DeviceId;
        typedef std::unordered_map<DeviceId, SoundFontId> SoundFontIdMap;
		FluidSynth();
		FluidSynth(const FluidSynth &&) = delete;
		FluidSynth &operator=(const FluidSynth &&) = delete;
		virtual ~FluidSynth();
		void send(const com::midi::Event &event, long double elapsedMillis);
		void seek(long double millis);
		void setCC(int ch, int cc, int value);
		virtual SoundFontId addSoundFont(const DeviceId& deviceId, const std::string &soundfontPath);
		const SoundFontIdMap& soundFontIds() const { return soundFontIdMap; }
	protected:
		SoundFontIdMap soundFontIdMap;
		virtual void initSynth();
		virtual void tearDownSynth();
		void initLibraryFunctions();
		virtual std::string findFluidSynthLibraryPath() const;
		void resetLibraryFunctions();
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
		std::function<fluid_synth_program_select_ftype> _fluid_synth_program_select;
		std::function<new_fluid_file_renderer_ftype> _new_fluid_file_renderer;
		std::function<fluid_file_renderer_process_block_ftype> _fluid_file_renderer_process_block;
		std::function<delete_fluid_file_renderer_ftype> _delete_fluid_file_renderer;
		std::function<new_fluid_player_ftype> _new_fluid_player;
		std::function<fluid_player_stop_ftype> _fluid_player_stop;
		std::function<fluid_player_set_playback_callback_ftype> _fluid_player_set_playback_callback;
		std::function<fluid_player_seek_ftype> _fluid_player_seek;
		std::function<fluid_player_play_ftype> _fluid_player_play;
		std::function<fluid_player_join_ftype> _fluid_player_join;
		std::function<fluid_player_get_current_tick_ftype> _fluid_player_get_current_tick;
		std::function<fluid_player_add_mem_ftype> _fluid_player_add_mem;
		std::function<delete_fluid_player_ftype> _delete_fluid_player;
		std::function<fluid_player_get_midi_tempo_ftype> _fluid_player_get_midi_tempo;
		std::function<fluid_player_get_division_ftype> _fluid_player_get_division;
		std::function<fluid_player_set_tick_callback_ftype> _fluid_player_set_tick_callback;

		fluid_settings_t *settings = nullptr;
		fluid_synth_t *synth = nullptr;
		fluid_audio_driver_t *adriver = nullptr;
		fluid_sequencer_t* seq = nullptr;
		fluid_seq_id_t synthSeqID = 0;
		bool midiEventToFluidEvent(const com::midi::Event& src, fluid_event_t& evt, bool doThrow = true);
		virtual void handleMetaEvent(const com::midi::Event& event);
		long double _playerOffsetMillis = 0;
	};
	typedef std::shared_ptr<FluidSynth> FluidSynthPtr;
}
