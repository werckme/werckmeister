#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="shorterThan">
    /// Selects any event whitch duration is shorter or equal than the given duration
    /// see also: [duration](#durationSelector), [longerThan](#longerThan).
    /// ```
    /// shorterThan(1) {...}
    /// ```
    /// </selector>
    /// <param name="duration" type="quarters"></param>
    class ShorterThan : public ISelector
    {
    public:
        ShorterThan(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ShorterThan() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
