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
#include <fm/units.hpp>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <fm/midi.hpp>
#include <list>
#include <unordered_map>
#include <fm/config/configServer.h>
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
		void play(fm::Ticks ticks);
		void panic();
		void stop();
		inline fm::BPM bpm() const { return std::max(bpm_, 1.0); }
		void bpm(fm::BPM bpm) { bpm_ = bpm; }
		fm::Ticks elapsed() const { return elapsed_; }
		void reset();
	private:
		void initOutputMap();
		void handleMetaEvent(const fm::midi::Event &ev);
		void changeDevice(const std::string &deviceId);
		typedef std::recursive_mutex Lock;
		typedef std::unordered_map<std::string, Output> OutputMap;
		Lock lock;
		std::unique_ptr<TTimer> playerTimer_;
		State state_ = Stopped;
		void onProcess();
		void updateTicks();
		Clock::time_point elapsed_time_;
		fm::BPM bpm_ = 120.0;
		fm::Ticks elapsed_ = 0;
		OutputMap outputMap_;
	};

	///////////////////////////////////////////////////////////////////////////////
	// IMPL
	template<class TBackend, class TMidiProvider, class TTimer>
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::MidiplayerClient()
	{
		initOutputMap();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::initOutputMap()
	{
		auto &cs = fm::getConfigServer();
		auto outputs = getOutputs();
		for(const auto &x : cs.getDevices()) {
			auto name = fm::to_string(x.first);
			auto id = x.second.deviceId;
			auto it = std::find_if(outputs.begin(), outputs.end(), [id](const auto &x) { return x.id == id; });
			if (it==outputs.end()) {
				throw std::runtime_error("output with id = " + id + " not found");
			}
			outputMap_[name] = *it; 
		}
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
		auto millis = duration_cast<duration<fm::Ticks, std::milli>>(t - elapsed_time_).count();
		elapsed_ += static_cast<fm::Ticks>( millis / (60 / (bpm() * fm::PPQ) * 1000) );
		elapsed_time_ = t;
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::panic()
	{
		for (fm::midi::Channel channel=0; channel <= fm::midi::MaxChannel; ++channel) {
			for (fm::midi::Pitch pitch=0; pitch <= fm::midi::MaxPitch; ++pitch)  {
				Backend::send(fm::midi::Event::NoteOff(channel, 0, pitch));
			}
		}
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::changeDevice(const std::string &deviceId)
	{

	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::handleMetaEvent(const fm::midi::Event &ev)
	{
		using namespace fm;
		if (ev.metaEventType() == midi::Tempo) {
			auto metaIntValue = midi::Event::MetaGetIntValue(ev.metaData(), ev.metaDataSize());
			bpm(midi::MicrosecondsPerMinute/(double)metaIntValue);
		}
		if (ev.metaEventType() == midi::CustomMetaEvent) {
			auto customEvent = midi::Event::MetaGetCustomData(ev.metaData(), ev.metaDataSize());
			if (customEvent.type == midi::CustomMetaData::SetDevice) {
				std::string deviceId(customEvent.data.begin(), customEvent.data.end());
				changeDevice(deviceId);
			}
		}
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::onProcess()
	{
		std::lock_guard<Lock> lockGuard(lock);
		typename MidiProvider::Events events;
		MidiProvider::getEvents(this->elapsed_, events, [this](const auto &ev) {
			if (ev.eventType() == fm::midi::MetaEvent) {
				this->handleMetaEvent(ev);
				return false;
			}
			return true;
		});
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
		elapsed_time_ = Clock::now();
		if (!playerTimer_) {
			playerTimer_ = std::make_unique<TTimer>([this]() {
				updateTicks();
				onProcess();
			});
		}
		playerTimer_->start(milliseconds(1));
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::play(fm::Ticks ticks)
	{
		std::lock_guard<Lock> lockGuard(lock);
		play();
		MidiProvider::seek(ticks);
		elapsed_ = ticks;
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
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::reset()
	{
		MidiProvider::reset();
	}


}
#endif