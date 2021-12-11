#ifndef FM_MIDIBACKENDCONTAINER_HPP
#define FM_MIDIBACKENDCONTAINER_HPP

#include <vector>
#include <com/midi.hpp>
#include <rtmidi/RtMidi.h>
#include "AMidiBackend.h"
#include <unordered_map>

namespace fmapp {
	class MidiBackendContainer : public AMidiBackend {
	public:
		typedef AMidiBackend Base;
		typedef std::unordered_map <std::string, AMidiBackendPtr> OutputIdToBackend;
		MidiBackendContainer();
		MidiBackendContainer(const MidiBackendContainer&&) = delete;
		MidiBackendContainer& operator=(const MidiBackendContainer&&) = delete;
		Outputs getOutputs() const;
		virtual ~MidiBackendContainer();
		virtual void send(const com::midi::Event &event, const Output *output) override;
		virtual void tearDown() override;
		virtual void panic() override;
		void addBackend(AMidiBackendPtr);
		const OutputIdToBackend& outputIdToBackend();
		AMidiBackendPtr getBackend(const std::string &outputId);
	private:
        std::vector<AMidiBackendPtr> _midiBackends;
		OutputIdToBackend _outputIdToBackend;
		void _initOutputIdToBackendMap();
	};
}

#endif