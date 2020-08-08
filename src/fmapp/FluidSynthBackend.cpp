#ifndef FM_FLUIDSYNTH_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include "FluidSynthBackend.h"

namespace fmapp {
	FluidSynthBackend::FluidSynthBackend()
	{
		_synths.push_back(std::make_shared<FluidSynth>("/usr/share/soundfonts/FluidR3_GM.sf2"));
	}
	FluidSynthBackend::Outputs FluidSynthBackend::getOutputs() const
	{
		Output output;
		output.id = "1";
		output.name = "fluid synth";
		output.portid = 1;
		FluidSynthBackend::Outputs result({output});
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