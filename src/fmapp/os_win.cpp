#include "os.hpp"
#include "windows.h"
#include "shlwapi.h"
#include <boost/filesystem.hpp>


namespace {
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired);
	BOOL WINAPI ConsoleHandler(DWORD signal);
}

namespace fmapp {
	namespace os {
		void MMTimer::start(std::chrono::milliseconds millis) 
		{
			if (handle_) {
				return;
			}
			auto due = static_cast<DWORD>(millis.count());
			::CreateTimerQueueTimer(
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
			::DeleteTimerQueueTimer(NULL, handle_, INVALID_HANDLE_VALUE); 
			handle_ = nullptr;
		}
	}
}

namespace fmapp {
	namespace os {
		namespace {
			SigtermHandler sigtermHandler_;
		}
		void setSigtermHandler(const SigtermHandler &sigtermHandler)
		{
			sigtermHandler_ = sigtermHandler;
			::SetConsoleCtrlHandler(ConsoleHandler, TRUE);
		}
		fm::String getExecutablePath()
		{
			CHAR szFileName[MAX_PATH + 1];
			GetModuleFileNameA(NULL, szFileName, MAX_PATH + 1);
			auto strPath = fm::String(szFileName);
			return boost::filesystem::path(strPath).parent_path().string();
		}
	}
}

namespace {
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		auto *callback = reinterpret_cast<fmapp::os::MMTimer::Callback*>(lpParam);
		(*callback)();
	}

	BOOL WINAPI ConsoleHandler(DWORD signal)
	{
		if (signal == CTRL_C_EVENT && !!fmapp::os::sigtermHandler_) {
			fmapp::os::sigtermHandler_();
		}
		return TRUE;
	}
}