#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    class ASelectorBase : public ISelector
    {
    public:
        ASelectorBase(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
       virtual ~ASelectorBase() = default;
    protected:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
