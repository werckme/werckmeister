#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
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
		virtual void send(const com::midi::Event &event, const Output *output) override;
		virtual void tearDown() override;
		virtual void panic() override;
		static void createInstance(const std::string &deviceId, const std::string &soundfontPath);

	private:
		static std::unordered_map<std::string, FluidSynthPtr> _synths;
	};
}
