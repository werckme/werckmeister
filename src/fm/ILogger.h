#ifndef ILOGGER_H
#define ILOGGER_H

#include <ostream>
#include <functional>
#include <memory>

#define WMLogLambda(x) [&](auto &log) { (x); } 

namespace fm {
    class ILogger {
    public:
        enum LogLevel { LevelDebug, LevelBabble, LevelWarn, LevelError, LevelNone };
        typedef std::function<void(std::ostream&)> WriteToStreamF;
        virtual void babble(const WriteToStreamF&) = 0;
        virtual void debug(const WriteToStreamF&) = 0;
        virtual void warn(const WriteToStreamF&) = 0;
        virtual void error(const WriteToStreamF&) = 0;
        virtual void logLevel(LogLevel lvl) = 0;
        virtual LogLevel logLevel() const = 0;
    };
    typedef std::shared_ptr<ILogger> ILoggerPtr;
}

#endif