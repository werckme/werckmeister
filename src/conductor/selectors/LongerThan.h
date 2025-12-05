#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="longerThan">
    /// Selects any event whitch duration is longer or equal than the given duration
    /// see also: [duration](#durationSelector), [shorterThan](#shorterThan).
    /// ```
    /// longerThan(1) {...}
    /// ```
    /// </selector>
    /// <param name="duration" type="quarters"></param>
    class LongerThan : public ISelector
    {
    public:
        LongerThan(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~LongerThan() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
