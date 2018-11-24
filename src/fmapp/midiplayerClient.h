#ifndef FMAPP_MIDIPLAYER_CLIENT_HPP
#define FMAPP_MIDIPLAYER_CLIENT_HPP

#include <vector>
#include <string>
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <loki/Singleton.h>
#pragma GCC diagnostic pop
#else
#include <loki/Singleton.h>
#endif

#include <memory>
#include <fm/config.hpp>
#include <chrono>
#include <mutex>
#include <algorithm>

namespace fmapp {

	template<class TBackend, class TMidiProvider, class TTimer>
	class MidiplayerClient : public TBackend, public TMidiProvider {
		friend struct Loki::CreateUsingNew<MidiplayerClient>;
	public:
		enum State { Undefined, Playing, Stopped };
		typedef TTimer Timer;
		typedef TBackend Backend;
		typedef typename Backend::Output Output;
		typedef typename Backend::Outputs Outputs;
		typedef TMidiProvider MidiProvider;
		typedef std::chrono::high_resolution_clock Clock;
		MidiplayerClient();
		MidiplayerClient(const MidiplayerClient&&) = delete;
		MidiplayerClient& operator=(MidiplayerClient&&) = delete;
		~MidiplayerClient() = default;
		Outputs getOutputs() const;
		bool setOutput(const Output &);
		bool isPlaying() const { return state_ == Playing; }
		void play();
		void stop();
		inline double bpm() const { return std::max(bpm_, 1.0); }
		fm::Ticks elapsed() const { return elapsed_; }
		void seek(fm::Ticks ticks);
		void reset();
	private:
		typedef std::recursive_mutex Lock;
		Lock lock;
		std::unique_ptr<TTimer> playerTimer_;
		State state_ = Stopped;
		void onProcess();
		void updateTicks();
		Clock::time_point started_;
		double bpm_ = 120.0;
		fm::Ticks elapsed_ = 0;
	};

	///////////////////////////////////////////////////////////////////////////////
	// IMPL
	template<class TBackend, class TMidiProvider, class TTimer>
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::MidiplayerClient()
	{
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::Outputs MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputs() const
	{
		return Backend::getOutputs();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	bool MidiplayerClient<TBackend, TMidiProvider, TTimer>::setOutput(const Output &output)
	{
		return Backend::setOutput(output);
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::updateTicks()
	{
		if (!isPlaying()) {
			return;
		}
		using namespace std::chrono;
		auto t = Clock::now();
		auto millis = duration_cast<duration<fm::Ticks, std::milli>>(t - started_).count();
		elapsed_ = static_cast<fm::Ticks>( millis / (60 / (bpm() * fm::PPQ) * 1000) );
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::onProcess()
	{
		std::lock_guard<Lock> lockGuard(lock);
		MidiProvider::Events events;
		MidiProvider::getEvents(this->elapsed_, events);
		Backend::send(events);
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::play()
	{
		if (isPlaying()) {
			return;
		}
		using namespace std::chrono;
		state_ = Playing;
		started_ = Clock::now();
		playerTimer_ = std::make_unique<TTimer>([this]() {
			updateTicks();
			onProcess();
		});
		playerTimer_->start(milliseconds(1));
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::stop()
	{
		if (!isPlaying()) {
			return;
		}
		state_ = Stopped;
		playerTimer_->stop();
		elapsed_ = 0;
		reset();
		playerTimer_.reset();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::reset()
	{
		MidiProvider::reset();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::seek(fm::Ticks ticks)
	{
		std::lock_guard<Lock> lockGuard(lock);
		int offset_ms = static_cast<int>((1000.0 * 60.0 / bpm()) * ((double)ticks / (double)fm::PPQ ));
		started_ = Clock::now() - std::chrono::milliseconds(offset_ms);
		MidiProvider::seek(ticks);
		elapsed_ = ticks;
	}

}
#endif