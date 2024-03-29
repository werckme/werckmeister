#pragma once

#include <com/ILogger.h>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    /**
     * a logger with the possiblity to add a related event to the message
     * (with this information we can deteremine where in the source the warning occurs)
     * (currently only warnings are supported)
     */
    class IEventLogger : public virtual com::ILogger
    {
    public:
        virtual void warn(const WriteToStreamF &, const documentModel::Event &theRelatedEvent) = 0;
    };
    typedef std::shared_ptr<IEventLogger> IEventLoggerPtr;
}
