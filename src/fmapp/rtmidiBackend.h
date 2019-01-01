#ifndef FM_RTMIDI_BACKEND_HPP
#define FM_RTMIDI_BACKEND_HPP

#include <memory>
#include <vector>
#include <fm/midi.hpp>
#include <rtmidi/RtMidi.h>

namespace fmapp {
	class RtMidiBackend {
	public:
		enum { UNKNOWN_PORT = -1 };
		struct Output {
			int portid = UNKNOWN_PORT;
			std::string id;
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
		void send(const fm::midi::Event &event, Output *output = nullptr);
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
		for (auto &ev : events) {
			send(ev, output);
		}
	}
}

#endif