#include "RTMidiBackend.h"
#include <algorithm>

namespace 
{
	void inputCallback(double timeStamp, std::vector<unsigned char> *message, void *userData);
}

namespace app
{
	RtMidiBackend::RtMidiBackend()
	{
		auto defaultOutput = std::make_unique<RtMidiOut>();
		auto nOutputs = defaultOutput->getPortCount();
		midiOuts.resize(std::max((int)nOutputs, (int)1));
		midiOuts[0].swap(defaultOutput);

		auto defaultInput = std::make_unique<RtMidiIn>();
		auto nInputs = defaultInput->getPortCount();
		midiIns.resize(std::max((int)nInputs, (int)1));
		midiIns[0].swap(defaultInput);
	}

	RtMidiOut *RtMidiBackend::getRtOutputReadyForSend(int idx)
	{
		if ((size_t)idx > midiOuts.size())
		{
			throw std::runtime_error("RtMidiBackend outputs: index out of bounds " + std::to_string(idx));
		}
		if (midiOuts.at(idx).get() == nullptr)
		{
			auto newOut = std::make_unique<RtMidiOut>();
			newOut->openPort(idx);
			midiOuts.at(idx).swap(newOut);
		}
		if (idx == 0)
		{ // default port exists already but is initial closed
			if (!defaultRtOutput()->isPortOpen())
			{
				defaultRtOutput()->openPort(0);
			}
		}
		return midiOuts.at(idx).get();
	}

	RtMidiBackend::~RtMidiBackend() = default;

	RtMidiBackend::Outputs RtMidiBackend::getOutputs() const
	{
		auto nOutputs = defaultRtOutput()->getPortCount();
		Outputs result(nOutputs);
		for (size_t idx = 0; idx < nOutputs; ++idx)
		{
			auto &output = result[idx];
			output.portid = idx;
			output.id = std::to_string(idx);
			output.name = defaultRtOutput()->getPortName(idx);
		}
		return result;
	}

	RtMidiBackend::Inputs RtMidiBackend::getInputs() const 
	{
		auto nInputs = defaultRtInput()->getPortCount();
		Inputs result(nInputs);
		for (size_t idx = 0; idx < nInputs; ++idx)
		{
			auto &input = result[idx];
			input.portid = idx;
			input.id = std::to_string(idx);
			input.name = defaultRtInput()->getPortName(idx);
		}
		return result;
	}

	void RtMidiBackend::panic()
	{
		com::Byte message[3] = {0};
		for (auto &rtOut : midiOuts)
		{
			if (rtOut.get() == nullptr || !rtOut->isPortOpen())
			{
				continue;
			}
			for (com::midi::Channel channel = 0; channel <= com::midi::MaxChannel; ++channel)
			{
				message[0] = static_cast<com::Byte>((0xB << 4) | channel);
				message[1] = 0x7B; // all notes off
				message[2] = 0;
				rtOut->sendMessage(&message[0], 3);
			}
		}
	}

	void RtMidiBackend::closeAllPorts()
	{
		for (auto &rtOut : midiOuts)
		{
			if (rtOut.get() == nullptr)
			{
				continue;
			}
			if (rtOut->isPortOpen())
			{
				continue;
			}
			rtOut->closePort();
		}
		for (auto &rtIn : midiIns)
		{
			if (rtIn.get() == nullptr)
			{
				continue;
			}
			if (rtIn->isPortOpen())
			{
				continue;
			}
			rtIn->closePort();
		}		
	}

	void RtMidiBackend::send(const com::midi::Event &ev, const Output *output, long double elapsedMillis)
	{
		if (output == nullptr)
		{
			output = &output_;
		}
		if (output->portid == UNKNOWN_PORT)
		{
			return;
		}

		auto port = getRtOutputReadyForSend(output->portid);
		const unsigned int StaticBufferSize = 255;
		com::Byte buffer[StaticBufferSize];
		std::vector<com::Byte> fallback;
		auto eventSize = ev.payloadSize(com::midi::MidiEventTargetDevice);
		com::Byte *bff = &buffer[0];
		if (eventSize > StaticBufferSize)
		{
			fallback.resize(eventSize, 0);
			bff = fallback.data();
		}

		ev.writePayload(bff, eventSize, com::midi::MidiEventTargetDevice);
		port->sendMessage(bff, eventSize);
	}

	void RtMidiBackend::listenTo(const Input *input) 
	{
		if (input->portid < 0 || input->portid >= midiIns.size())
		{
			throw std::runtime_error("invalid port id: " + std::to_string(input->portid));
		}
		auto rtIn = midiIns.at(input->portid).get();
		if (rtIn == nullptr)
		{
			auto newIn = std::make_unique<RtMidiIn>();
			newIn->openPort(input->portid);
			rtIn = newIn.get();
			midiIns.at(input->portid).swap(newIn);
		}
		if (!rtIn->isPortOpen())
		{
			rtIn->openPort(input->portid);
		}
		rtIn->setCallback(&inputCallback, (void*)input);
	}
}


namespace 
{
	void inputCallback(double timeStamp, std::vector<unsigned char> *message, void *userData)
	{
		const auto *input = reinterpret_cast<app::AMidiBackend::Input*>(userData);
		if (!input->midiMessageCallback)
		{
			return;
		}
		input->midiMessageCallback(message);
	}
}