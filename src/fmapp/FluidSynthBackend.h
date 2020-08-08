#ifndef FM_FLUIDSYNTH_BACKEND_HPP
#define FM_FLUIDSYNTH_BACKEND_HPP

#include <memory>
#include <functional>
#include <vector>
#include <fm/midi.hpp>
#include "FluidSynthWrapper.h"
#include "AMidiBackend.h"

namespace fmapp {
	class FluidSynthBackend : public AMidiBackend {
	public:
		FluidSynthBackend();
		FluidSynthBackend(const FluidSynthBackend&&) = delete;
		FluidSynthBackend& operator=(const FluidSynthBackend&&) = delete;
		virtual Outputs getOutputs() const override;
		virtual ~FluidSynthBackend();
		virtual void send(const fm::midi::Event &event, const Output *output) override;
		virtual void tearDown() override;
		virtual void panic() override;
	private:
		std::vector<FluidSynthPtr> _synths;
	};
}

#endif