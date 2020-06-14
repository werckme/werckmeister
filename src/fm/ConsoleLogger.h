#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

namespace fm {
    class ConsoleLogger : public virtual ILogger {
    public:
        typedef std::function<void(std::ostream&)> WriteToStreamF;
        virtual void babble(const WriteToStreamF&);
        virtual void warn(const WriteToStreamF&);
        virtual void error(const WriteToStreamF&);
        virtual void writeMessage(const WriteToStreamF &f);
        virtual ~ConsoleLogger() = default;
        virtual void logLevel(LogLevel lvl) { _logLevel = lvl; }
        virtual LogLevel logLevel() const { return _logLevel; }
    private:
        LogLevel _logLevel = LevelNone;
    };
}

#endif