#pragma once

#include <memory>
#include <vector>
#include <com/midi.hpp>
#include <rtmidi/RtMidi.h>
#include "AMidiBackend.h"

namespace app
{
	class RtMidiBackend : public AMidiBackend
	{
	public:
		RtMidiBackend();
		RtMidiBackend(const RtMidiBackend &&) = delete;
		RtMidiBackend &operator=(const RtMidiBackend &&) = delete;
		virtual Outputs getOutputs() const override;
		virtual ~RtMidiBackend();
		virtual void send(const com::midi::Event &event, const Output *output, long double elapsedMillis) override;
		virtual void tearDown() override { closeAllPorts(); }
		virtual void panic() override;

	private:
		void closeAllPorts();
		typedef std::vector<std::unique_ptr<RtMidiOut>> MidiOuts;
		MidiOuts midiOuts;
		inline RtMidiOut *defaultRtOutput() const { return midiOuts.at(0).get(); }
		RtMidiOut *getRtOutputReadyForSend(int idx);
		Output output_;
	};
}
