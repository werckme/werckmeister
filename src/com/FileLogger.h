#pragma once

#include "ILogger.h"
#include <ICompilerProgramOptions.h>

#include <ostream>

namespace com
{
    class FileLogger : public virtual ILogger
    {
    public:
        FileLogger(std::string logFilePath) : _logFilePath(logFilePath) {}
        typedef std::function<void(std::ostream &)> WriteToStreamF;
        virtual void debug(const WriteToStreamF &) override;
        virtual void babble(const WriteToStreamF &) override;
        virtual void warn(const WriteToStreamF &) override;
        virtual void error(const WriteToStreamF &) override;
        virtual void writeMessage(const WriteToStreamF &f, std::ostream &os);
        virtual ~FileLogger() = default;
        virtual void logLevel(LogLevel lvl) override { _logLevel = lvl; }
        virtual LogLevel logLevel() const override { return _logLevel; }
        
    private:
        LogLevel _logLevel = LevelWarn;
        std::string _logFilePath;
    };
}
