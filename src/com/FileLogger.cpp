#include "FileLogger.h"
#include <fstream>
#include <iostream>

namespace com
{
    void FileLogger::debug(const WriteToStreamF &f)
    {
        if (_logLevel > LevelDebug)
        {
            return;
        }
        std::fstream flog(_programOptions->logFile(), std::ios::app);
        flog << "[debug] ";
        writeMessage(f, flog);
    }
    void FileLogger::babble(const WriteToStreamF &f)
    {
        if (_logLevel > LevelBabble)
        {
            return;
        }
        std::fstream flog(_programOptions->logFile(), std::ios::app);
        flog << "[info] ";
        writeMessage(f, flog);
    }
    void FileLogger::warn(const WriteToStreamF &f)
    {
        if (_logLevel > LevelWarn)
        {
            return;
        }
        std::fstream flog(_programOptions->logFile(), std::ios::app);
        flog << "[warning] ";
        writeMessage(f, flog);
    }
    void FileLogger::error(const WriteToStreamF &f)
    {
        if (_logLevel > LevelError)
        {
            return;
        }
        std::fstream flog(_programOptions->logFile(), std::ios::app);
        flog << "[error] ";
        writeMessage(f, flog);
    }
    void FileLogger::writeMessage(const WriteToStreamF &f, std::ostream &os)
    {
        f(os);
        os << std::endl;
    }
}
