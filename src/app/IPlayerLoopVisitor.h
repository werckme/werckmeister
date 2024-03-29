#pragma once

#include <com/common.hpp>
#include <memory>
namespace app
{
    class IPlayerLoopVisitor
    {
    public:
        virtual void visit(com::Ticks elapsed) = 0;
        virtual void loopBegin() = 0;
        virtual void loopEnd() = 0;
        virtual ~IPlayerLoopVisitor() = default;
    };
    typedef std::shared_ptr<IPlayerLoopVisitor> IPlayerLoopVisitorPtr;
}
