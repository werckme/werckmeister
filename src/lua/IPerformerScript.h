#pragma once

#include <memory>
#include <com/common.hpp>

namespace lua
{
    class IPerformerScript 
    {
    public:
        virtual ~IPerformerScript() = default;
    };
    typedef std::shared_ptr<IPerformerScript> IPerformerScriptPtr;
}