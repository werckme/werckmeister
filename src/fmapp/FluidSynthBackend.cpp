#ifndef FM_FLUIDSYNTH_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include "FluidSynthBackend.h"

namespace fmapp {

	std::unordered_map<std::string, FluidSynthPtr> FluidSynthBackend::_synths;

	FluidSynthBackend::FluidSynthBackend()
	{
	}

	void FluidSynthBackend::createInstance(const std::string& deviceId, const std::string& soundfontPath)
	{
		_synths.insert(std::make_pair(deviceId, std::make_shared<FluidSynth>(soundfontPath)));
	}

	FluidSynthBackend::Outputs FluidSynthBackend::getOutputs() const
	{
		FluidSynthBackend::Outputs result;
		result.reserve(_synths.size());
		for (const auto& idSynthPair : _synths) {
			Output output;
			output.id = idSynthPair.first;
			output.name = output.id;
			output.portid = 0;
			result.emplace_back(output);
		}
		return result;
	}
	FluidSynthBackend::~FluidSynthBackend()
	{

	}
	void FluidSynthBackend::send(const com::midi::Event &event, const Output *output)
	{
		_synths.begin()->second->send(event);
	}
	void FluidSynthBackend::tearDown()
	{
		_synths.clear();
	}
	void FluidSynthBackend::panic()
	{

	}
}

#endif