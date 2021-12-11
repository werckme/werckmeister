#pragma once

#include "IEventLogger.h"
#include "Warning.hpp"
#include <sstream>
#include "IWarningsCollection.h"

namespace documentModel {
	struct Event;
	namespace compiler {
        template <class TLoggerBase>
        class LoggerAndWarningsCollector : public TLoggerBase, public IEventLogger, public IWarningsCollection {
        private:
            Warnings _warnings;
        public:
            typedef TLoggerBase Base;
            virtual void warn(const ILogger::WriteToStreamF&, const Event &theRelatedEvent) override;
            virtual const Warnings & warnings() const override { return _warnings; }
            virtual ~LoggerAndWarningsCollector() = default;
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
            _warnings.push_back(warning);
        }
    }
}
