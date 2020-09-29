#include "ConsoleLogger.h"
#include <iostream>

namespace fm {
    void ConsoleLogger::debug(const WriteToStreamF& f)
    {
        if (_logLevel > LevelDebug) {
            return;
        }
        std::clog << "[debug] ";
        writeMessage(f, std::clog);
    }    
    void ConsoleLogger::babble(const WriteToStreamF& f)
    {
        if (_logLevel > LevelBabble) {
            return;
        }
        std::clog << "[info] ";
        writeMessage(f, std::clog);
    }
    void ConsoleLogger::warn(const WriteToStreamF& f)
    {
        if (_logLevel > LevelWarn) {
            return;
        }
        std::clog << "[warning] ";
        writeMessage(f, std::clog);
    }
    void ConsoleLogger::error(const WriteToStreamF& f)
    {
        if (_logLevel > LevelError) {
            return;
        }
        std::cerr << "[error] ";
        writeMessage(f, std::cerr);
    }
    void ConsoleLogger::writeMessage(const WriteToStreamF &f, std::ostream &os)
    {
        f(os);
        os << std::endl;
    }
}
