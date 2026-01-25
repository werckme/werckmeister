#pragma once

#include <vector>
#include <string>
#include <memory>
#include <com/config.hpp>
#include <com/units.hpp>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <com/midi.hpp>
#include <list>
#include <unordered_map>
#include <com/config/configServer.h>
#include <algorithm>
#include <iostream>
#include <functional>

namespace app
{

	template <class TBackend, class TMidiProvider, class TTimer>
	class MidiplayerClient : public TBackend, public TMidiProvider
	{
	public:
		enum State
		{
			Undefined,
			Playing,
			Stopped
		};
		typedef TTimer Timer;
		typedef TBackend Backend;
		typedef typename Backend::Output Output;
		typedef typename Backend::Outputs Outputs;
		struct OutputInfo
		{
			Output output;
			int offset = 0;
		};
		typedef TMidiProvider MidiProvider;
		typedef typename MidiProvider::TrackId TrackId;
		typedef std::chrono::high_resolution_clock Clock;
		typedef std::function<void()> OnEnd;
		typedef std::function<void(const com::midi::Event*)> OnSendMidiEvent;
		OnEnd onEnd = OnEnd();
		OnSendMidiEvent onSendMidiEvent = nullptr;
		MidiplayerClient();
		MidiplayerClient(const MidiplayerClient &&) = delete;
		MidiplayerClient &operator=(MidiplayerClient &&) = delete;
		~MidiplayerClient() = default;
		Outputs getOutputs() const;
		bool isPlaying() const { return state_ == Playing; }
		void play();
		void play(com::Ticks ticks);
		void seek(com::Ticks ticks);
		void panic();
		void stop();
		com::Ticks elapsed() const { return MidiProvider::millisToTicks(elapsedMillis_); }
		void reset();
		void send(const com::midi::Event &ev, const Output *output = nullptr);
		inline com::BPM bpm() const { return MidiProvider::bpm(); }
		void bpm(com::BPM bpm);
		void updateOutputMapping(const com::ConfigServer::Devices &devices);
		OutputInfo getOutputInfo(const std::string &deviceName) const;
		com::Ticks end = com::Ticks(-1);
	private:
		bool _seeked = false;
		const OutputInfo *getOutputInfo() const;
		void handleMetaEvent(const com::midi::Event &ev);
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
		typename Timer::Ptr playerTimer_;
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
	template <class TBackend, class TMidiProvider, class TTimer>
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::MidiplayerClient()
	{
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::bpm(com::BPM bpm)
	{
		auto elapsedTicks = elapsed();
		MidiProvider::bpm(bpm);
		elapsedMillis_ = MidiProvider::ticksToMillis(elapsedTicks);
	}

	namespace 
	{
		template<class OutputContainer>
		auto _findOutputById(const OutputContainer& outputs, const com::DeviceConfig::DeviceId &id)
		{
			auto it = std::find_if(outputs.begin(), outputs.end(), [id](const auto &ouput)
				{ 
					return ouput.id == id; 
				}
			);
			return it;
		}
		template<class OutputContainer>
		auto _findOutputByName(const OutputContainer& outputs, const com::String&name)
		{
			auto it = std::find_if(outputs.begin(), outputs.end(), [name](const auto &ouput)
				{ 
					return ouput.name.find(name) != std::string::npos; 
				}
			);
			return it;
		}
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::updateOutputMapping(const com::ConfigServer::Devices &devices)
	{
		outputMap_.clear();
		auto outputs = getOutputs();
		for (const auto &device : devices)
		{
			const auto &deviceDefName = device.first;
			const auto &portId = device.second.deviceId;
			const auto &deviceName = device.second.deviceName;
			typename Outputs::const_iterator it;
			if (!portId.empty())
			{
				it = _findOutputById(outputs, portId);
				if (it == outputs.end())
				{
					throw std::runtime_error("output with port id = " + portId + " not found");
				}
			}
			if (!deviceName.empty())
			{
				it = _findOutputByName(outputs, deviceName);
				if (it == outputs.end())
				{
					throw std::runtime_error("output with device name = " + deviceName + " not found");
				}
			}
			if (it == outputs.end())
			{
				throw std::runtime_error("not able to assign an output of device definiton: " + deviceDefName);
			}
			OutputInfo inf;
			inf.output = *it;
			inf.offset = device.second.offsetMillis;
			outputMap_[deviceDefName] = inf;
		}
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::OutputInfo
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputInfo(const std::string &deviceName) const
	{
		auto it = outputMap_.find(deviceName);
		if (it == outputMap_.end())
		{
			return OutputInfo();
		}
		return it->second;
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::Outputs MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputs() const
	{
		return Backend::getOutputs();
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::updateElapsedTime()
	{
		if (!isPlaying())
		{
			return;
		}
		using namespace std::chrono;
		auto t = Clock::now();
		elapsedMillis_ += duration_cast<duration<com::Ticks, std::milli>>(t - elapsed_time_).count();
		elapsed_time_ = t;
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::panic()
	{
		Backend::panic();
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::changeDevice(const std::string &deviceUname)
	{
		if (currentTrack_ == MidiProvider::INVALID_TRACKID)
		{
			return;
		}
		auto outputIt = outputMap_.find(deviceUname);
		if (outputIt == outputMap_.end())
		{
			return;
		}
		trackOutputs_[currentTrack_] = outputIt->second;
		trackOffsets_[currentTrack_] = outputIt->second.offset;
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	const typename MidiplayerClient<TBackend, TMidiProvider, TTimer>::OutputInfo *
	MidiplayerClient<TBackend, TMidiProvider, TTimer>::getOutputInfo() const
	{
		auto it = trackOutputs_.find(currentTrack_);
		if (it == trackOutputs_.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::handleMetaEvent(const com::midi::Event &ev)
	{
		using namespace com;
		if (ev.metaEventType() == midi::Tempo)
		{
			auto metaIntValue = midi::Event::MetaGetIntValue(ev.metaData(), ev.metaDataSize());
			bpm(midi::MicrosecondsPerMinute / (double)metaIntValue);
		}
		if (ev.metaEventType() == midi::CustomMetaEvent)
		{
			auto customEvent = midi::Event::MetaGetCustomData(ev.metaData(), ev.metaDataSize());
			if (customEvent.type == midi::CustomMetaData::SetDevice)
			{
				std::string deviceId(customEvent.data.begin(), customEvent.data.end());
				changeDevice(deviceId);
			}
		}
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::send(const com::midi::Event &ev, const Output *output)
	{
		if (ev.eventType() == com::midi::MetaEvent)
		{
			this->handleMetaEvent(ev);
			return;
		}
		if (output == nullptr)
		{
			auto outputInfo = getOutputInfo();
			output = &outputInfo->output;
		}
		Backend::send(ev, output, this->elapsedMillis_);
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::onProcess()
	{
		if (this->end >= 0 && this->elapsed() >= this->end)
		{
			onEnd();
			return;
		}
		std::lock_guard<Lock> lockGuard(lock);
		typename MidiProvider::Events events;
		auto t = this->elapsedMillis_;
		MidiProvider::getEvents(this->elapsedMillis_, events, trackOffsets_);
		for (const auto &evAndTrack : events)
		{
			currentTrack_ = evAndTrack.trackId;
			const auto &ev = evAndTrack.event;
			if (onSendMidiEvent)
			{
				onSendMidiEvent(&ev);
				if (_seeked)
				{
					_seeked = false;
					break;
				}
			}
			send(ev);
		}
		currentTrack_ = MidiProvider::INVALID_TRACKID;
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::play()
	{
		if (isPlaying())
		{
			return;
		}
		using namespace std::chrono;
		state_ = Playing;
		elapsed_time_ = Clock::now();
		if (!playerTimer_)
		{
			playerTimer_ = Timer::Create([this]()
			{
				updateElapsedTime();
				try
				{
					onProcess();
				}
				catch (const std::exception &ex)
				{
					std::cerr << ex.what() << std::endl;
				}
				catch (...)
				{
					std::cerr << __FILE__ << " unexpected exception" << std::endl;
				}
			});
		}
		playerTimer_->start(milliseconds(1));
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::play(com::Ticks ticks)
	{
		std::lock_guard<Lock> lockGuard(lock);
		MidiProvider::iterate([this, ticks](com::Ticks pos, const typename MidiProvider::Event &ev) 
		{ 	// consume all events except NoteOn and NoteOff
			if (pos >= ticks)
			{
					currentTrack_ = MidiProvider::INVALID_TRACKID;
					return false; // aka break
			}
			if (ev.event.eventType() == com::midi::NoteOn || ev.event.eventType() == com::midi::NoteOff)
			{
					return true; // aka continue
			}
			currentTrack_ = ev.trackId;
			send(ev.event);
			return true;
		});
		elapsedMillis_ = MidiProvider::ticksToMillis(ticks);
		Backend::seek(elapsedMillis_);
		MidiProvider::seek(elapsedMillis_, trackOffsets_);
		play();
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::seek(com::Ticks ticks)
	{
		elapsedMillis_ = MidiProvider::ticksToMillis(ticks);
		Backend::seek(elapsedMillis_);
		MidiProvider::seek(elapsedMillis_, trackOffsets_);
		_seeked = true;
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::stop()
	{
		if (!isPlaying())
		{
			return;
		}
		state_ = Stopped;
		playerTimer_->stop();
		elapsedMillis_ = 0;
		reset();
	}

	template <class TBackend, class TMidiProvider, class TTimer>
	void MidiplayerClient<TBackend, TMidiProvider, TTimer>::reset()
	{
		MidiProvider::reset();
	}

}
