#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="onBar">
    ///     Selects an event in a given bar number or a list of bar numbers.
    ///     TODO: should be renamed into inBar.
    /// ```
    /// onBar(1 3) {...}
    /// ```
    /// see also: [fromBar](#fromBar), [toBar](#toBar), [nthBar](#nthBar).
    /// </selector>
    /// <param name="beat" type="bar number+"></param>
    class OnBar : public ISelector
    {
    public:
        OnBar(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) 
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~OnBar() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
