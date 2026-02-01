#pragma once

#include <memory>
#include <vector>
#include <com/midi.hpp>
#include <myrtmidi/RtMidi.h>
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
		virtual void init() override {}
		virtual void tearDown() override { closeAllPorts(); }
		virtual void panic() override;
		//////////
		virtual Inputs getInputs() const override;
		virtual void listenTo(const Input *input) override;

	private:
		void closeAllPorts();
		typedef std::vector<std::unique_ptr<RtMidiOut>> MidiOuts;
		MidiOuts midiOuts;
		typedef std::vector<std::unique_ptr<RtMidiIn>> MidiIns;
		MidiIns midiIns;		
		inline RtMidiOut *defaultRtOutput() const { return midiOuts.at(0).get(); }
		inline RtMidiIn *defaultRtInput() const { return midiIns.at(0).get(); }
		RtMidiOut *getRtOutputReadyForSend(int idx);
		Output output_;
	};
}
