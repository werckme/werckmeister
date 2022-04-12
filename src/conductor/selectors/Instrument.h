#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="instrumentSelector">
    /// Selects any event which belongs to one of the given instruments.
    /// *there is a known [issue](https://github.com/werckme/werckmeister/issues/230) related to instrumentSection*
    /// #### example:
    /// ```
    /// instrument(bass) {...}\n
    /// instrument(bass drums) {...}\n
    /// ```
    /// </selector>
    /// <param name="instrument name" type="instrumentName+"></param>
    class Instrument : public ISelector
    {
    public:
        Instrument(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Instrument() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
