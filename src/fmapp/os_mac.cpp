#include "os.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <mach-o/dyld.h>

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
        fm::String _getExecutablePath()
        {
        	char path[1024];
                uint32_t size = sizeof(path);
                if (_NSGetExecutablePath(&path[0], &size) == 0) {
		  return fm::String(&path[0]);
		}
		char* dynamicStr = new char[size];
		if (_NSGetExecutablePath(dynamicStr, &size) != 0) {
		  delete[] dynamicStr;
		  throw std::runtime_error("get executable path failed");
		}
		std::string result(dynamicStr);
		delete[] dynamicStr;
		return result;
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
		  auto strPath = _getExecutablePath();
		  return boost::filesystem::path(strPath).parent_path().string();
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
