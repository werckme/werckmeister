#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="toPosition">
    ///     Selects any event where its position is before a given position in quarters.
    /// see also: [toPosition](#toPosition).
    /// ## example, select the events before time &lt; 8 quarters:
    /// ```
    /// toPosition(8) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters"></param>
    class ToPosition : public ISelector
    {
    public:
        ToPosition(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToPosition() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
