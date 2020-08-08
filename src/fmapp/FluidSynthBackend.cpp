#ifndef FM_FLUIDSYNTH_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include "FluidSynthBackend.h"

namespace fmapp {
	FluidSynthBackend::FluidSynthBackend()
	{

	}
	FluidSynthBackend::Outputs FluidSynthBackend::getOutputs() const
	{
		FluidSynthBackend::Outputs result;
		return result;
	}
	FluidSynthBackend::~FluidSynthBackend()
	{

	}
	void FluidSynthBackend::send(const fm::midi::Event &event, const Output *output)
	{

	}
	void FluidSynthBackend::tearDown()
	{

	}
	void FluidSynthBackend::panic()
	{

	}
}

#endif