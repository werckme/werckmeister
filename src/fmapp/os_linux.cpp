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
		fm::String getExecutablePath()
		{
			char *bff = readlink_malloc("/proc/self/exe");
			if (bff == NULL) {
				throw std::runtime_error("Error resolving symlink /proc/self/exe.");
			}
			auto result = boost::filesystem::path(bff);
			free(bff);
			return result.parent_path().string();
		}
	}
}

namespace {
	void signalCallback(int signal)
	{
		fmapp::os::sigtermHandler_();
	}
}
