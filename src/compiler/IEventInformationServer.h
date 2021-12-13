#pragma once

#include <memory>

namespace documentModel
{
    struct Event;
}
namespace compiler
{
    class IEventInformationServer
    {
    public:
        virtual ~IEventInformationServer() = default;
    };
    typedef std::shared_ptr<IEventInformationServer> IEventInformationServerPtr;
}
