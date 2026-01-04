#ifndef FM_FLUIDSYNTH_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include "FluidSynthBackend.h"
#include <map>

namespace app
{
	FluidSynthBackend::FluidSynthBackend()
	{
	}

	void FluidSynthBackend::addSoundFont(const std::string &deviceId, const std::string &soundfontPath)
	{
		if (!_fluidSynth)
		{
			throw std::runtime_error("fluid synth not ready");
		}
		_fluidSynth->addSoundFont(deviceId, soundfontPath);
	}

	FluidSynthBackend::Outputs FluidSynthBackend::getOutputs() const
	{
		FluidSynthBackend::Outputs result;
		const auto& soundfonts = _fluidSynth->soundFontIds();
		result.reserve(soundfonts.size());
		for (const auto &idSynthPair : soundfonts)
		{
			Output output;
			output.id = idSynthPair.first;
			output.name = output.id;
			output.portid = idSynthPair.second;
			result.emplace_back(output);
		}
		return result;
	}
	FluidSynthBackend::~FluidSynthBackend()
	{
	}
	void FluidSynthBackend::send(const com::midi::Event &event, const Output *output, long double elapsedMillis)
	{
		if (event.eventType() == com::midi::Controller && event.parameter1() == 0) // msb
		{
			_fluidSynth->setMsb(event.channel(), event.parameter2());
			return;
		}
		if (event.eventType() == com::midi::Controller && event.parameter1() == 32) // lsb
		{
			_fluidSynth->setLsb(event.channel(), event.parameter2());
			return;
		}
		if (event.eventType() == com::midi::ProgramChange)
		{
			_fluidSynth->setPreset(output->portid, event.channel(), event.parameter1());
			return;
		}
		_fluidSynth->send(event, elapsedMillis);
	}
	void FluidSynthBackend::init()
	{
		_fluidSynth = std::make_unique<app::FluidSynth>();
	}
	void FluidSynthBackend::tearDown()
	{
		_fluidSynth.reset();
	}
	void FluidSynthBackend::panic()
	{
	}
	void FluidSynthBackend::seek(long double millis)
	{
		_fluidSynth->seek(millis);
	}
}

#endif