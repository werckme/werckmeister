#ifndef FM_AMIDIBACKEND_HPP
#define FM_AMIDIBACKEND_HPP

#include <memory>
#include <vector>
#include <fm/midi.hpp>


namespace fmapp {
	class AMidiBackend {
	public:
		enum { UNKNOWN_PORT = -1 };
		struct Output {
			typedef std::string IdType;
			int portid = UNKNOWN_PORT;
			IdType id;
			IdType backendId;
			std::string name = "undefined output";
		};
		typedef std::vector<Output> Outputs;
		virtual ~AMidiBackend() = default;
        template<class TEvents>
		void send(const TEvents &events, const Output *output);
		virtual Outputs getOutputs() const = 0;
		virtual void send(const fm::midi::Event &event, const Output *output) = 0;
		virtual void tearDown() = 0;
		virtual void panic() = 0;
	};
	typedef std::shared_ptr<AMidiBackend> AMidiBackendPtr;
	///////////////////////////////////////////////////////////////////////////////
	// Impl.
	template<class TEvents>
	void AMidiBackend::send(const TEvents &events, const Output *output)
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