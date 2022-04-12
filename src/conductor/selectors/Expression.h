#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>
#include <compiler/EventInformationServer.h>

namespace conductor
{
    /// <selector name="expression">
    ///     Select events via its applied expressions.
    /// ```
    /// expression(p) {...}
    /// expression(p f ff) {...}
    /// ```
    /// </selector>
    /// <param name="expression" type="expression+"></param>
    class Expression : public ISelector
    {
    public:
        Expression(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Expression() = default;
    protected:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
