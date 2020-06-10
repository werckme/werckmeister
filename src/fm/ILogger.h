#ifndef ILOGGER_H
#define ILOGGER_H

#include <ostream>
#include <functional>

#define WRMLogLambda(x) [&](auto &log) { (x); } 

namespace fm {
    class ILogger {
    public:
        typedef std::function<void(std::ostream&)> WriteToStreamF;
        virtual void babble(const WriteToStreamF&) = 0;
        virtual void warn(const WriteToStreamF&) = 0;
        virtual void error(const WriteToStreamF&) = 0;
    };
}

#endif