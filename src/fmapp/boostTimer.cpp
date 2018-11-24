#include "boostTimer.h"
#include <boost/bind.hpp>

namespace fmapp {

	namespace {
		boost::asio::io_context io;
	}

	void BoostTimer::start(std::chrono::milliseconds millis)
	{
		t_ = std::make_unique<boost::asio::steady_timer>(io, boost::asio::chrono::milliseconds(millis));
		t_->async_wait(boost::bind(&BoostTimer::onCallback, this));
	}

	void BoostTimer::stop()
	{
		std::lock_guard<Lock> lock(lock_);
		t_->cancel();
		t_.reset();
	}

	void BoostTimer::onCallback() {
		std::lock_guard<Lock> lock(lock_);
		if (!t_) {
			return;
		}
		if (!!callback_) {
			callback_();
		}
		t_->async_wait(boost::bind(&BoostTimer::onCallback, this));
	}

	void BoostTimer::io_run()
	{
		io.run();
	}

}