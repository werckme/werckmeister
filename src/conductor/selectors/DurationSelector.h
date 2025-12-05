#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="durationSelector">
    /// Selects any event whitch duration matches to the given duration
    /// see also: [longerThan](#longerThan), [shorterThan](#shorterThan).
    /// ```
    /// duration(1) {...}
    /// ```
    /// </selector>
    /// <param name="duration" type="quarters"></param>
    class DurationSelector : public ISelector
    {
    public:
        DurationSelector(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~DurationSelector() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
