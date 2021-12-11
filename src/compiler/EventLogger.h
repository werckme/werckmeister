#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include "IEventLogger.h"
#include <com/ConsoleLogger.h>
#include "Warning.hpp"
#include <sstream>
#include <forward.hpp>
#include <com/tools.h>

namespace documentModel {
	struct Event;
	namespace compiler {
        template <class TLoggerBase>
        class EventLogger : public TLoggerBase, public IEventLogger {
        private:
            DocumentPtr _document;
        public:
            EventLogger(DocumentPtr document) : _document(document) {}
            typedef TLoggerBase Base;
            virtual void warn(const ILogger::WriteToStreamF&, const Event &theRelatedEvent);
        };
        ///////////////////////////////////////////////////////////////////////
        template <class TLoggerBase>
        void EventLogger<TLoggerBase>::warn(const ILogger::WriteToStreamF& f, const Event &sourceObject)
        {
            std::stringstream messageStream;
            f(messageStream);
            TLoggerBase::warn([&](auto &log) {  
                com::documentMessage(log, _document, sourceObject.sourceId, sourceObject.sourcePositionBegin, messageStream.str());
            });
        }
    }
}

#endif