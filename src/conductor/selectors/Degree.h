#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="degree">
    ///     Selects an MIDI event which was rendered via a template degree.
    /// ```
    /// degree(I III) {...}
    /// ```
    /// </selector>
    /// <param name="degree" type="degree+"></param>
    class Degree : public ISelector
    {
    public:
        Degree(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Degree() = default;

    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
