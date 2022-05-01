#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="fromBeat">
    ///Selects an event where its beat time is equal or greater.
    // see also: [toBeat](#toBeat), [onBeat](#onBeat).
    /// ```
    /// fromBeat(1) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters"></param>
    class FromBeat : public ISelector
    {
    public:
        FromBeat(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~FromBeat() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
