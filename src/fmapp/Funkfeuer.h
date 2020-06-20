#ifndef FMAPP_FUNKFEUER_HPP
#define FMAPP_FUNKFEUER_HPP


#include "IPlayerLoopVisitor.h"
#include <fm/ILogger.h>
#include "TimelineVisitor.hpp"
#include <memory>
#include <fm/exception.hpp>
#include "JsonWriterBase.h"
#include <forward.hpp>

namespace fmapp {
    /**
     * funkfeuer sends event informations via udp. Used for example by the vscode extension
     * to obtain the informations which events are currently played.   
     */
    class Funkfeuer : public JsonWriterBase, public IPlayerLoopVisitor {
    private:
        fm::ILoggerPtr              _logger;
        fmapp::DefaultTimelinePtr   _timelineVisitor;
    public:
        typedef JsonWriterBase Base;
        Funkfeuer(sheet::compiler::ICompilerVisitorPtr compilerVisitor,  fm::ILoggerPtr logger)
            :_logger(logger)
            , _timelineVisitor(std::dynamic_pointer_cast<fmapp::DefaultTimeline>(compilerVisitor))
        {
            if (!_timelineVisitor)
            FM_THROW(fm::Exception, "expected a time line visitor instance");
            lastTimelineEvent = _timelineVisitor->intervalContainer().end();
        }
        virtual void visit(fm::Ticks elapsed) override;  
        virtual ~Funkfeuer() = default;
    private:
        fmapp::EventTimeline::const_iterator lastTimelineEvent;
    };
}
#endif