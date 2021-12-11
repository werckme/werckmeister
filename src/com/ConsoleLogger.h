#pragma once

#include "ILogger.h"
#include <ostream>

namespace com {
    class ConsoleLogger : public virtual ILogger {
    public:
        typedef std::function<void(std::ostream&)> WriteToStreamF;
        virtual void debug(const WriteToStreamF&) override;
        virtual void babble(const WriteToStreamF&) override;
        virtual void warn(const WriteToStreamF&) override;
        virtual void error(const WriteToStreamF&) override;
        virtual void writeMessage(const WriteToStreamF &f, std::ostream &os);
        virtual ~ConsoleLogger() = default;
        virtual void logLevel(LogLevel lvl) override  { _logLevel = lvl; }
        virtual LogLevel logLevel() const override  { return _logLevel; }
    private:
        LogLevel _logLevel = LevelWarn;
    };
}

