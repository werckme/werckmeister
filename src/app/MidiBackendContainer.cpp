#include "MidiBackendContainer.h"

namespace app
{

    MidiBackendContainer::MidiBackendContainer()
    {
    }

    void MidiBackendContainer::addBackend(AMidiBackendPtr backend)
    {
        _midiBackends.push_back(backend);
    }

    AMidiBackend::Outputs MidiBackendContainer::getOutputs() const
    {
        AMidiBackend::Outputs result;
        for (const auto &backend : this->_midiBackends)
        {
            auto outputs = backend->getOutputs();
            result.insert(result.end(), outputs.begin(), outputs.end());
        }
        return result;
    }
    MidiBackendContainer::~MidiBackendContainer()
    {
    }
    void MidiBackendContainer::send(const com::midi::Event &event, const Output *_output, long double elapsedMillis)
    {
        if (_output == nullptr)
        {
            return;
        }
        auto backend = getBackend(_output->id);
        if (backend == nullptr)
        {
            throw new std::runtime_error("unexcpected midi backend error");
        }
        backend->send(event, _output, elapsedMillis);
    }
    void MidiBackendContainer::seek(long double millis)
    {
        for (const auto &backend : this->_midiBackends)
        {
            backend->seek(millis);
        }
    }
    void MidiBackendContainer::init()
    {
        for (const auto &backend : this->_midiBackends)
        {
            backend->init();
        }
    }
    void MidiBackendContainer::tearDown()
    {
        for (const auto &backend : this->_midiBackends)
        {
            backend->tearDown();
        }
    }
    void MidiBackendContainer::panic()
    {
        for (const auto &backend : this->_midiBackends)
        {
            backend->panic();
        }
    }

    const MidiBackendContainer::OutputIdToBackend &MidiBackendContainer::outputIdToBackend()
    {
        if (_outputIdToBackend.empty())
        {
            _initOutputIdToBackendMap();
        }
        return _outputIdToBackend;
    }

    void MidiBackendContainer::_initOutputIdToBackendMap()
    {
        for (const auto &backend : this->_midiBackends)
        {
            auto outputs = backend->getOutputs();
            for (const auto &output : outputs)
            {
                _outputIdToBackend[output.id] = backend;
            }
        }
    }

    AMidiBackendPtr MidiBackendContainer::getBackend(const std::string &outputId)
    {
        const auto &outputMap = outputIdToBackend();
        const auto &outputMapIt = outputMap.find(outputId);
        if (outputMapIt == outputMap.end())
        {
            return nullptr;
        }
        return outputMapIt->second;
    }
}