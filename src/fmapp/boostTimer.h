#ifndef BOOST_TIMER_IMPL_H
#define BOOST_TIMER_IMPL_H

#include <functional>
#include <chrono>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>

namespace fmapp {
	struct BoostTimer {
	public:
		typedef std::function<void()> Callback;
		BoostTimer(const Callback &callback) : callback_(callback) {}
		void start(std::chrono::milliseconds millis);
		void stop();
		static void io_run();
	private:
		typedef std::recursive_mutex Lock;
		void *handle_ = nullptr;
		Callback callback_;
		void onCallback();
		std::unique_ptr<boost::asio::steady_timer> t_;
		Lock lock_;
	};
}

#endif