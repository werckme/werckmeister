#include "boostTimer.h"
#include <boost/bind/bind.hpp>

#include <mutex>
#include <memory>
#include <list>
#include <boost/asio/executor_work_guard.hpp>

namespace app
{

	namespace
	{
		typedef std::lock_guard<BoostTimer::Lock> LockGuard;
		std::list<BoostTimer::Ptr> _gc;
		boost::asio::io_context io;
		auto work_ = boost::asio::make_work_guard(io);
	}

	BoostTimer::BoostTimer(const Callback &callback) : callback_(callback)
	{
	}

	void BoostTimer::start(std::chrono::milliseconds millis)
	{
		LockGuard lock(lock_);
		if (t_)
		{
			stop();
		}
		t_ = std::make_unique<boost::asio::steady_timer>(io, boost::asio::chrono::milliseconds(millis));
		t_->async_wait(boost::bind(&BoostTimer::onCallback, this));
	}

	void BoostTimer::stop()
	{
		callback_ = nullptr;
		LockGuard lock(lock_);
		t_->cancel();
		t_->wait();
		t_.reset();
	}

	void BoostTimer::onCallback()
	{
		LockGuard lock(lock_);
		if (!t_ || !callback_)
		{
			return;
		}
		else
		{
			callback_();
		}
		t_->async_wait(boost::bind(&BoostTimer::onCallback, this));
	}

	void BoostTimer::io_run()
	{
		io.run();
	}

	void BoostTimer::io_stop()
	{
		work_.reset();
	}

	void BoostTimer::cleanup()
	{
		_gc.clear();
	}

	BoostTimer::Ptr BoostTimer::Create(const Callback &callback)
	{
		auto result = std::make_shared<BoostTimer>(callback);
		_gc.push_back(result);
		return result;
	}
}