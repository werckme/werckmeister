#ifndef FM_RTMIDI_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include <memory>
#include <vector>
#include <fm/midi.hpp>
#include <RtMidi.h>

namespace fmapp {
	class RtMidiBackend {
	public:
		enum { UNKNOWN_PORT = -1 };
		struct Output {
			int id = UNKNOWN_PORT;
			std::string name = "undefined output";
		};
		typedef std::vector<Output> Outputs;
		RtMidiBackend();
		RtMidiBackend(const RtMidiBackend&&) = delete;
		RtMidiBackend& operator=(const RtMidiBackend&&) = delete;
		Outputs getOutputs() const;
		bool setOutput(const Output &output);
		virtual ~RtMidiBackend();
		template<class TEvents>
		void send(const TEvents &events, Output *output = nullptr);
	private:
		std::unique_ptr<RtMidiOut> midiout_;
		Output output_;
	};

	///////////////////////////////////////////////////////////////////////////////
	// Impl.
	template<class TEvents>
	void RtMidiBackend::send(const TEvents &events, Output *output)
	{
		if (events.empty()) {
			return;
		}
		if (output == nullptr) {
			output = &output_;
		}
		if (output->id == UNKNOWN_PORT) {
			return;
		}
		if (!midiout_->isPortOpen()) {
			midiout_->openPort(output->id);
		}
		std::vector<fm::Byte> bytes;
		bytes.reserve(3);
		for (auto &ev : events) {
			bytes.resize(ev.payloadSize(), 0);
			ev.writePayload(bytes.data(), bytes.size());
			midiout_->sendMessage(&bytes);
		}
	}
}

#endif