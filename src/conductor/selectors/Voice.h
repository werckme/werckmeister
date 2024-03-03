#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="voiceSelector">
    /// Selects any event which is located in one of the given voice numbers.
    /// #### example:
    /// ```
    /// ------------ sheet file\n
    /// [\n
    /// {\n
    ///   -- voice 1\n
    /// }\n
    /// {\n
    ///  -- voice 1\n
    /// }\n
    /// ]\n
    /// ------------ conductions file:\n
    /// voice(1) {...}\n
    /// voice(1 2) {...}\n
    /// ```
    /// </selector>
    /// <param name="voice number" type="number+"></param>
    class Voice : public ISelector
    {
    public:
        Voice(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Voice() = default;
    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
