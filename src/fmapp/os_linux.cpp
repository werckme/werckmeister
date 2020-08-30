#include "os.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <unistd.h>

namespace {
	void signalCallback(int signal);
	char * readlink_malloc (const char *filename)
	{
		// http://www.delorie.com/gnu/docs/glibc/libc_279.html
		int size = 255;
		char *buffer = NULL;

		while (1) {
			buffer = (char *) realloc (buffer, size);
			int nchars = readlink (filename, buffer, size);
			if (nchars < 0) {
				free (buffer);
				return NULL;
			}
			if (nchars < size) {
				buffer[nchars] = 0;
				return buffer;
			}
			size *= 2;
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
			struct sigaction sigIntHandler;
			sigIntHandler.sa_handler = &signalCallback;
			sigemptyset(&sigIntHandler.sa_mask);
			sigIntHandler.sa_flags = 0;

			sigaction(SIGINT, &sigIntHandler, NULL);
		}
		fm::String getExecutablePath()
		{
#ifdef __EMSCRIPTEN__
			return "";
#endif
			char *bff = readlink_malloc("/proc/self/exe");
			if (bff == NULL) {
				throw std::runtime_error("Error resolving symlink /proc/self/exe.");
			}
			auto result = boost::filesystem::path(bff);
			free(bff);
			return result.parent_path().string();
		}
		int getPId()
		{
			return static_cast<int>(::getpid());
		}
	}
}

namespace {
	void signalCallback(int signal)
	{
		fmapp::os::sigtermHandler_();
	}
}
