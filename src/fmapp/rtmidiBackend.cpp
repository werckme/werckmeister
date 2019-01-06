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
			output.portid = idx;
			output.id = std::to_string(idx);
			output.name = midiout_->getPortName(idx);
		}
		return result;
	}

	bool RtMidiBackend::setOutput(const Output &output)
	{
		this->output_ = output;
		return true;
	}

	void RtMidiBackend::send(const fm::midi::Event &ev, const Output *output)
	{
		if (output == nullptr) {
			output = &output_;
		}
		if (output->portid == UNKNOWN_PORT) {
			return;
		}
		
		if (!midiout_->isPortOpen()) {
			midiout_->openPort(output->portid);
		}
		const unsigned int StaticBufferSize = 255;
		fm::Byte buffer[StaticBufferSize];
		std::vector<fm::Byte> fallback;
		auto eventSize = ev.payloadSize();
		fm::Byte *bff = &buffer[0];
		if (eventSize > StaticBufferSize) {
			fallback.resize(eventSize, 0);
			bff = fallback.data();
		}
		
		ev.writePayload(bff, eventSize);
		midiout_->sendMessage(bff, eventSize);
	}
}
