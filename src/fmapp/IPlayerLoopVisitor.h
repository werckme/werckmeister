#ifndef FMAPP_IPLAYERLOOPVISITOR_HPP
#define FMAPP_IPLAYERLOOPVISITOR_HPP


#include <fm/common.hpp>
#include <memory>
namespace fmapp {
    class IPlayerLoopVisitor {
    public:
        virtual void visit(fm::Ticks elapsed) = 0;
        virtual ~IPlayerLoopVisitor() = default;
    };
    typedef std::shared_ptr<IPlayerLoopVisitor> IPlayerLoopVisitorPtr;
}
#endif