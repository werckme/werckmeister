#include "os.hpp"
#include <signal.h>
//#include <unistd.h>

namespace {
	void signalCallback(int signal);
}

namespace fmapp {
	namespace os {
		namespace {
			SigtermHandler sigtermHandler_;
		}
		void setSigtermHandler(const SigtermHandler &sigtermHandler)
		{
			sigtermHandler_ = sigtermHandler;
			struct sigaction sigIntHandler;
			sigIntHandler.sa_handler = &signalCallback;
			sigemptyset(&sigIntHandler.sa_mask);
			sigIntHandler.sa_flags = 0;

			sigaction(SIGINT, &sigIntHandler, NULL);
		}
	}
}

namespace {
	void signalCallback(int signal)
	{
		fmapp::os::sigtermHandler_();
	}
}
