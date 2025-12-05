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
        std::fstream flog(_logFilePath, std::ios::app | std::ios::out);
        flog << "[debug] ";
        writeMessage(f, flog);
    }
    void FileLogger::babble(const WriteToStreamF &f)
    {
        if (_logLevel > LevelBabble)
        {
            return;
        }
        std::fstream flog(_logFilePath, std::ios::app | std::ios::out);
        flog << "[info] ";
        writeMessage(f, flog);
    }
    void FileLogger::warn(const WriteToStreamF &f)
    {
        if (_logLevel > LevelWarn)
        {
            return;
        }
        std::fstream flog(_logFilePath, std::ios::app | std::ios::out);
        flog << "[warning] ";
        writeMessage(f, flog);
    }
    void FileLogger::error(const WriteToStreamF &f)
    {
        if (_logLevel > LevelError)
        {
            return;
        }
        std::fstream flog(_logFilePath, std::ios::app | std::ios::out);
        flog << "[error] ";
        writeMessage(f, flog);
    }
    void FileLogger::writeMessage(const WriteToStreamF &f, std::ostream &os)
    {
        f(os);
        os << std::endl;
    }
}
