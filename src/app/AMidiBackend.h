#pragma once

#include <memory>
#include <vector>
#include <com/midi.hpp>
#include <functional>

namespace app
{
	class AMidiBackend
	{
	public:
		enum
		{
			UNKNOWN_PORT = -1
		};
		struct Output
		{
			typedef std::string IdType;
			int portid = UNKNOWN_PORT;
			IdType id;
			std::string name = "undefined output";
		};
		struct Input
		{
			typedef std::vector<com::Byte> MessageData;
			typedef std::function<void(const MessageData*)> MidiMessageCallback;
			typedef std::string IdType;
			int portid = UNKNOWN_PORT;
			IdType id;
			std::string name = "undefined input";
			MidiMessageCallback midiMessageCallback;
		};
        typedef std::vector<Input> Inputs;
		typedef std::vector<Output> Outputs;
		virtual ~AMidiBackend() = default;
		template <class TEvents>
		void send(const TEvents &events, const Output *output);
		virtual Outputs getOutputs() const = 0;
		virtual void send(const com::midi::Event &event, const Output *output, long double elapsedMillis) = 0;
		virtual void seek(long double millis) {}
		virtual void init() = 0;
		virtual void tearDown() = 0;
		virtual void panic() = 0;
		//////////
		virtual Inputs getInputs() const { return Inputs(); }
		virtual void listenTo(const Input *input) {}
	};
	typedef std::shared_ptr<AMidiBackend> AMidiBackendPtr;
	///////////////////////////////////////////////////////////////////////////////
	// Impl.
	template <class TEvents>
	void AMidiBackend::send(const TEvents &events, const Output *output)
	{
		if (events.empty())
		{
			return;
		}
		for (auto &ev : events)
		{
			send(ev, output);
		}
	}
}
