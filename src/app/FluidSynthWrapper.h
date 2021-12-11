#ifndef FM_FLUIDSYNTHWRAPPER_BACKEND_HPP
#define FM_FLUIDSYNTHWRAPPER_BACKEND_HPP

#include <memory>
#include <vector>
#include <fluidsynth.h>
#include <boost/dll.hpp>
#include <com/midi.hpp>

#if FLUIDSYNTH_VERSION_MAJOR < 2
#error "Fluidsynth version >2 required"
#endif

#define new_fluid_settings_ftype 			fluid_settings_t* ()
#define new_fluid_synth_ftype 				fluid_synth_t* (fluid_settings_t*)
#define new_fluid_audio_driver_ftype 		fluid_audio_driver_t* (fluid_settings_t*, fluid_synth_t*)
#define fluid_synth_sfload_ftype 		 	int(fluid_synth_t*, const char*, int)
#define fluid_synth_noteon_ftype 		 	int(fluid_synth_t*, int, int, int)
#define fluid_synth_noteoff_ftype 		 	int(fluid_synth_t*, int, int)
#define fluid_synth_program_change_ftype 	int(fluid_synth_t*, int, int)
#define fluid_synth_cc_ftype			 	int(fluid_synth_t*, int, int, int)
#define fluid_synth_pitch_bend_ftype	 	int(fluid_synth_t*, int, int)
#define fluid_settings_setstr_ftype 	  	int(fluid_settings_t*, const char*, const char*)
#define fluid_audio_driver_register_ftype 	int(const char**)
#define delete_fluid_audio_driver_ftype 	void(fluid_audio_driver_t*)
#define delete_fluid_synth_ftype 			void(fluid_synth_t*)
#define delete_fluid_settings_ftype 		void(fluid_settings_t*)

namespace app {
	class FluidSynth {
	public:
		FluidSynth(const std::string &soundfontPath);
		FluidSynth(const FluidSynth&&) = delete;
		FluidSynth& operator=(const FluidSynth&&) = delete;
		virtual ~FluidSynth();
		void send(const com::midi::Event& event);
	private:
        void initSynth(const std::string soundFondPath);
        void tearDownSynth();
        void initLibraryFunctions();
        std::string findFluidSynthLibraryPath() const;
		std::function<new_fluid_settings_ftype> 		 _new_fluid_settings;
		std::function<new_fluid_synth_ftype> 			 _new_fluid_synth;
		std::function<new_fluid_audio_driver_ftype> 	 _new_fluid_audio_driver;
		std::function<fluid_synth_sfload_ftype> 		 _fluid_synth_sfload;
		std::function<fluid_synth_noteon_ftype> 		 _fluid_synth_noteon;
		std::function<fluid_synth_noteoff_ftype> 		 _fluid_synth_noteoff;
		std::function<fluid_synth_program_change_ftype>  _fluid_synth_program_change;
		std::function<fluid_synth_cc_ftype>				 _fluid_synth_cc;
		std::function<fluid_synth_pitch_bend_ftype>		 _fluid_synth_pitch_bend;
		std::function<fluid_settings_setstr_ftype> 		 _fluid_settings_setstr;
		std::function<fluid_audio_driver_register_ftype> _fluid_audio_driver_register;
		std::function<delete_fluid_audio_driver_ftype> 	 _delete_fluid_audio_driver;
		std::function<delete_fluid_synth_ftype> 		 _delete_fluid_synth;
		std::function<delete_fluid_settings_ftype> 		 _delete_fluid_settings;
        boost::dll::shared_library *_library = nullptr;
        fluid_settings_t* settings = nullptr;
        fluid_synth_t* synth = nullptr;
        fluid_audio_driver_t* adriver = nullptr;
	};
    typedef std::shared_ptr<FluidSynth> FluidSynthPtr;
}

#endif