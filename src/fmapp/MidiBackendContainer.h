#ifndef FM_MIDIBACKENDCONTAINER_HPP
#define FM_MIDIBACKENDCONTAINER_HPP

#include <vector>
#include <fm/midi.hpp>
#include <rtmidi/RtMidi.h>
#include "AMidiBackend.h"

namespace fmapp {
	class MidiBackendContainer : public AMidiBackend {
	public:
		MidiBackendContainer();
		MidiBackendContainer(const MidiBackendContainer&&) = delete;
		MidiBackendContainer& operator=(const MidiBackendContainer&&) = delete;
		Outputs getOutputs() const;
		virtual ~MidiBackendContainer();
		virtual void send(const fm::midi::Event &event, const Output *output) override;
		virtual void tearDown() override;
		virtual void panic() override;
	private:
        std::vector<AMidiBackendPtr> _midiBackends;
	};
}

#endif