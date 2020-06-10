#include "ConsoleLogger.h"
#include <iostream>

namespace fm {
    void ConsoleLogger::babble(const WriteToStreamF& f)
    {
        std::cout << "[info] ";
        writeMessage(f);
    }
    void ConsoleLogger::warn(const WriteToStreamF& f)
    {
        std::cout << "[warning] ";
        writeMessage(f);
    }
    void ConsoleLogger::error(const WriteToStreamF& f)
    {
        std::cout << "[error] ";
        writeMessage(f);
    }
    void ConsoleLogger::writeMessage(const WriteToStreamF &f)
    {
        f(std::cout);
        std::cout << std::endl;
    }
}
