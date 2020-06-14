#ifndef LOGGERANDWARNINGSCOLLECTOR_H
#define LOGGERANDWARNINGSCOLLECTOR_H

#include "IEventLogger.h"
#include "Warning.hpp"
#include <sstream>

namespace sheet {
	struct Event;
	namespace compiler {
        template <class TLoggerBase>
        class LoggerAndWarningsCollector : public TLoggerBase, public IEventLogger {
        private:
        public:
            Warnings warnings;
            typedef TLoggerBase Base;
            virtual void warn(const ILogger::WriteToStreamF&, const Event &theRelatedEvent);
        };
        ///////////////////////////////////////////////////////////////////////
        template <class TLoggerBase>
        void LoggerAndWarningsCollector<TLoggerBase>::warn(const ILogger::WriteToStreamF& f, const Event &sourceObject)
        {
            std::stringstream messageStream;
            f(messageStream);
            Warning warning;
            warning.message = messageStream.str();
            warning.sourceObject = sourceObject;
            warnings.push_back(warning);
        }
    }
}
#endif