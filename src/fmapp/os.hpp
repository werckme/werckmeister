#ifndef FM_OS_HPP
#define FM_OS_HPP

#include "fm/common.hpp"
#include <functional>
#include <chrono>
#include <memory>

namespace fmapp {
	namespace os {		
		class MMTimer {
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
		void setSigtermHandler(const SigtermHandler&);
		fm::String getExecutablePath();
		int getPId();
	}
}

#endif
