#include "rtmidiBackend.h"

namespace fmapp {

	RtMidiBackend::RtMidiBackend() : midiout_(std::make_unique<RtMidiOut>()) 
	{
	}

	RtMidiBackend::~RtMidiBackend() = default;

	RtMidiBackend::Outputs RtMidiBackend::getOutputs() const
	{
		auto nOutputs = midiout_->getPortCount();
		Outputs result(nOutputs);
		for (size_t idx = 0; idx < nOutputs; ++idx) {
			auto &output = result[idx];
			output.id = idx;
			output.name = midiout_->getPortName(idx);
		}
		return result;
	}

	bool RtMidiBackend::setOutput(const Output &output)
	{
		if (output_.id != UNKNOWN_PORT && midiout_->isPortOpen()) {
			midiout_->closePort();
		}
		this->output_ = output;
		return true;
	}
}
