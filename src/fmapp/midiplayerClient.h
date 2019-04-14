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
		struct OutputInfo {
			Output output;
			int offset = 0;
		};
		typedef TMidiProvider MidiProvider;
		typedef typename MidiProvider::TrackId TrackId;
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
		fm::Ticks elapsed() const { return MidiProvider::millisToTicks(elapsedMillis_); }
		void reset();
		void send(const fm::midi::Event &ev, const Output *output = nullptr);
		inline fm::BPM bpm() const { return MidiProvider::bpm(); }
		void bpm(fm::BPM bpm);
		void updateOutputMapping(const fm::ConfigServer::Devices &devices);
		OutputInfo getOutputInfo(const std::string &deviceName) const;
	private:
		const OutputInfo * getOutputInfo() const;
		void handleMetaEvent(const fm::midi::Event &ev);
		void changeDevice(const std::string &deviceId);
		typedef std::recursive_mutex Lock;
		/**
		 * device name and assigned output
		 */
		typedef std::unordered_map<std::string, OutputInfo> OutputMap;
		/**
		 * trackid and assigned output
		 */
		typedef std::unordered_map<TrackId, OutputInfo> TrackOutputMap;
		typedef std::unordered_map<TrackId, int> TrackOffsets;
		Lock lock;
		std::unique_ptr<TTimer> playerTimer_;
		State state_ = Stopped;
		void onProcess();
		void updateElapsedTime();
		Clock::time_point elapsed_time_;
		typename MidiProvider::Millis elapsedMillis_ = 0;
		OutputMap outputMap_;
		TrackOutputMap trackOutputs_;
		TrackId currentTrack_ = MidiProvider::INVALID_TRACKID;
		TrackOffsets trackOffsets_;
	};

	///////////////////////////////////////////////////////////////////////////////
	// IMPL
	template<class TBackend, class TMidiProvider, class TTimer>
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::MidiplayerClient()
	{
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::bpm(fm::BPM bpm)
	{
		auto elapsedTicks = elapsed();
		MidiProvider::bpm(bpm);
		elapsedMillis_ = MidiProvider::ticksToMillis(elapsedTicks);
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::updateOutputMapping(const fm::ConfigServer::Devices &devices)
	{
		outputMap_.clear();
		auto outputs = getOutputs();
		for(const auto &x : devices) {
			auto name = fm::to_string(x.first);
			auto id = x.second.deviceId;
			auto it = std::find_if(outputs.begin(), outputs.end(), [id](const auto &x) { return x.id == id; });
			if (it==outputs.end()) {
				throw std::runtime_error("output with id = " + id + " not found");
			}
			OutputInfo inf;
			inf.output = *it;
			inf.offset = x.second.offsetMillis;
			outputMap_[name] = inf;
		}
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::OutputInfo
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputInfo(const std::string &deviceName) const
	{
		auto it = outputMap_.find(deviceName);
		if (it == outputMap_.end()) {
			return OutputInfo();
		}
		return it->second;
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
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::updateElapsedTime()
	{
		if (!isPlaying()) {
			return;
		}
		using namespace std::chrono;
		auto t = Clock::now();
		elapsedMillis_ += duration_cast<duration<fm::Ticks, std::milli>>(t - elapsed_time_).count();
		elapsed_time_ = t;
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::panic()
	{
		Backend::panic();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::changeDevice(const std::string &deviceId)
	{
		if (currentTrack_ == MidiProvider::INVALID_TRACKID) {
			return;
		}
		auto outputIt = outputMap_.find(deviceId);
		if (outputIt == outputMap_.end()) {
			return;
		}
		trackOutputs_[currentTrack_] = outputIt->second;
		trackOffsets_[currentTrack_] = outputIt->second.offset;
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	const typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::OutputInfo * 
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputInfo() const
	{
		auto it = trackOutputs_.find(currentTrack_);
		if (it == trackOutputs_.end()) {
			return nullptr;
		}
		return &(it->second);
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
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::send(const fm::midi::Event &ev, const Output *output)
	{
		if (ev.eventType() == fm::midi::MetaEvent) {
			this->handleMetaEvent(ev);
			return;
		}
		if (output == nullptr) {
			auto outputInfo = getOutputInfo();
			output = &outputInfo->output;
		}
		Backend::send(ev, output);
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::onProcess()
	{
		std::lock_guard<Lock> lockGuard(lock);
		typename MidiProvider::Events events;
		MidiProvider::getEvents(this->elapsedMillis_, events, trackOffsets_);
		for(const auto &evAndTrack : events) {
			currentTrack_ = evAndTrack.trackId;		
			const auto &ev = evAndTrack.event;
			send(ev);
		}
		currentTrack_ = MidiProvider::INVALID_TRACKID;
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
				updateElapsedTime();
				onProcess();
			});
		}
		playerTimer_->start(milliseconds(1));
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::play(fm::Ticks ticks)
	{
		std::lock_guard<Lock> lockGuard(lock);
		MidiProvider::iterate([this, ticks](fm::Ticks pos, const typename MidiProvider::Event &ev) 
		{	// consume all events except NoteOn and NoteOff
			if (pos >= ticks) {
				currentTrack_ = MidiProvider::INVALID_TRACKID;
				return false; // aka break
			}
			if (ev.event.eventType() == fm::midi::NoteOn
				|| ev.event.eventType() == fm::midi::NoteOff)
			{
				return true; // aka continue
			}
			currentTrack_ = ev.trackId;
			send(ev.event);
			return true;
		});
		play();
		elapsedMillis_ = MidiProvider::ticksToMillis(ticks);
		MidiProvider::seek(elapsedMillis_, trackOffsets_);
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::stop()
	{
		if (!isPlaying()) {
			return;
		}
		state_ = Stopped;
		playerTimer_->stop();
		elapsedMillis_ = 0;
		reset();
	}

	template<class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::reset()
	{
		MidiProvider::reset();
	}


}
#endif