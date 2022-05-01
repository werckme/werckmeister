#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="fromBar">
    ///     Selects an event where its bar time is equal or greater.
    /// ```
    /// fromBar(1) {...}
    /// ```
    /// see also: [toBar](#toBar), [onBar](#onBar), [nthBar](#nthBar).
    /// </selector>
    /// <param name="bar" type="bar number"></param>
    class FromBar : public ISelector
    {
    public:
        FromBar(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~FromBar() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
