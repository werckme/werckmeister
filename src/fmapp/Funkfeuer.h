#ifndef FMAPP_FUNKFEUER_HPP
#define FMAPP_FUNKFEUER_HPP


#include "IPlayerLoopVisitor.h"
#include <fm/ILogger.h>
#include "TimelineVisitor.hpp"
#include <memory>
#include <fm/exception.hpp>
#include "JsonWriterBase.h"
#include <forward.hpp>
#include "IStringSender.hpp"

namespace fmapp {
    /**
     * funkfeuer sends event informations via udp. Used for example by the vscode extension
     * to obtain the informations which events are currently played.   
     */
    class Funkfeuer : public JsonWriterBase, public IPlayerLoopVisitor {
    private:
        fm::ILoggerPtr         _logger;
        DefaultTimelinePtr     _timelineVisitor;
        IStringSenderPtr       _sender;
    public:
        typedef JsonWriterBase Base;
        Funkfeuer(sheet::compiler::ICompilerVisitorPtr compilerVisitor,
            fm::ILoggerPtr logger,
            IStringSenderPtr sender)
            :_logger(logger)
            , _timelineVisitor(std::dynamic_pointer_cast<fmapp::DefaultTimeline>(compilerVisitor))
            , _sender(sender)
        {
            if (!_timelineVisitor)
            FM_THROW(fm::Exception, "expected a time line visitor instance");
            lastTimelineEvent = _timelineVisitor->intervalContainer().end();
        }
        virtual void loopBegin() override {}
        virtual void loopEnd() override {}
        virtual void visit(fm::Ticks elapsed) override;  
        virtual ~Funkfeuer() = default;
    private:
        fmapp::EventTimeline::const_iterator lastTimelineEvent;
        unsigned long lastUpdateTimestamp = 0;
    };
}
#endif