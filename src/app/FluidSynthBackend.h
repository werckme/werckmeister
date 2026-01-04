#pragma once

#include <memory>
#include <functional>
#include <com/midi.hpp>
#include "FluidSynthWrapper.h"
#include "AMidiBackend.h"

namespace app
{
	class FluidSynthBackend : public AMidiBackend
	{
	public:
		FluidSynthBackend();
		FluidSynthBackend(const FluidSynthBackend &&) = delete;
		FluidSynthBackend &operator=(const FluidSynthBackend &&) = delete;
		virtual Outputs getOutputs() const override;
		virtual ~FluidSynthBackend();
		virtual void send(const com::midi::Event &event, const Output *output, long double elapsedMillis) override;
		virtual void seek(long double millis) override;
		virtual void init() override;
		virtual void tearDown() override;
		virtual void panic() override;
		void addSoundFont(const std::string &deviceId, const std::string &soundfontPath);
	private:
		std::unique_ptr<app::FluidSynth> _fluidSynth;
	};
}
