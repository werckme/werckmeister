#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="degree">
    ///     Selects a MIDI event which was created during rendering a template and its degrees.
    ///     Octaves of a degree will not be considered.
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
