#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="templateSelector">
    /// Selects any event which belongs to one of the given template names.
    /// #### example:
    /// ```
    /// template(myTemplate) {...}\n
    /// template(partI partII) {...}\n
    /// ```
    /// </selector>
    /// <param name="template name" type="templateName+"></param>
    class Template : public ISelector
    {
    public:
        Template(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Template() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
