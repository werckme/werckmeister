#include "MidiBackendContainer.h"



namespace fmapp {
    AMidiBackend::Outputs MidiBackendContainer::getOutputs() const
    {
        AMidiBackend::Outputs result;
        for(const auto backend : this->_midiBackends) {
            auto outputs = backend->getOutputs();
            result.insert(result.end(), outputs.begin(), outputs.end());
        }
    }
    MidiBackendContainer::~MidiBackendContainer()
    {
    }
    void MidiBackendContainer::send(const fm::midi::Event &event, const Output *output)
    {
        // TODO: need a mapping from output to backend
    }
    void MidiBackendContainer::tearDown()
    {
        for(const auto backend : this->_midiBackends) {
            backend->tearDown();
        } 
    }
    void MidiBackendContainer::panic()
    {
        for(const auto backend : this->_midiBackends) {
            backend->panic();
        } 
    }
}