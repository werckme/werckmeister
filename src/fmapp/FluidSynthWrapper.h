#ifndef FM_FLUIDSYNTHWRAPPER_BACKEND_HPP
#define FM_FLUIDSYNTHWRAPPER_BACKEND_HPP

#include <memory>
#include <vector>
#include <fluidsynth.h>

namespace fmapp {
	class FluidSynth {
	public:
		FluidSynth();
		FluidSynth(const FluidSynth&&) = delete;
		FluidSynth& operator=(const FluidSynth&&) = delete;
		virtual ~FluidSynth();
	private:

	};
}

#endif