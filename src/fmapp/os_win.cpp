#ifdef WIN32
#include "os.hpp"
#include "windows.h"
#include "shlwapi.h"

namespace {
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired);
}

namespace fmapp {
	namespace os {

		void MMTimer::start(std::chrono::milliseconds millis) 
		{
			if (handle_) {
				return;
			}
			auto due = millis.count();
			CreateTimerQueueTimer(
				&handle_,
				NULL,
				&WaitOrTimerCallback,
				&this->callback_,
				due,
				due,
				WT_EXECUTEDEFAULT
			);
		}
		void MMTimer::stop()
		{
			if (!handle_) {
				return;
			}
			// accoding to doc:
			// If this parameter is INVALID_HANDLE_VALUE, the function 
			// waits for any running timer callback functions to complete before returning.
			DeleteTimerQueueTimer(NULL, handle_, INVALID_HANDLE_VALUE); 
			handle_ = nullptr;
		}

	}
}

namespace {
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		auto *callback = reinterpret_cast<fmapp::os::MMTimer::Callback*>(lpParam);
		(*callback)();
	}
}


#endif