#pragma once

#include <functional>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <mutex>

namespace app {
	struct BoostTimer {
	public:
		typedef std::recursive_mutex Lock;
		typedef std::function<void()> Callback;
		BoostTimer(const Callback &callback);
		
		void start(std::chrono::milliseconds millis);
		void stop();
		static void io_run();
		static void io_stop();
	private:
		void *handle_ = nullptr;
		Callback callback_;
		void onCallback();
		std::unique_ptr<boost::asio::steady_timer> t_;
		Lock lock_;
	};
}

