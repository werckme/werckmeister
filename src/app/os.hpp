#pragma once

#include "com/common.hpp"
#include <functional>
#include <chrono>
#include <memory>

namespace app
{
	namespace os
	{
		class MMTimer
		{
		public:
			typedef std::function<void()> Callback;
			MMTimer(const Callback &callback) : callback_(callback) {}
			void start(std::chrono::milliseconds millis);
			void stop();

		private:
			void *handle_ = nullptr;
			Callback callback_;
		};

		typedef std::function<void()> SigtermHandler;
		void setSigtermHandler(const SigtermHandler &);
		com::String getExecutablePath();
		int getPId();
	}
}
