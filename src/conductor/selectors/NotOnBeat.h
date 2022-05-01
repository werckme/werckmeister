#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="notOnBeat">
    ///     Selects an event which does not appear on a given beat or a list of beats
    /// ```
    /// notOnBeat(1 3) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters+"></param>
    class NotOnBeat : public ISelector
    {
    public:
        NotOnBeat(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~NotOnBeat() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
