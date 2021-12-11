#pragma once


#include "IPlayerLoopVisitor.h"

namespace app {
    class PlayerTimePrinter : public IPlayerLoopVisitor {
    public:
        virtual void loopBegin() override {}
        virtual void loopEnd() override;
        virtual void visit(com::Ticks elapsed) override;  
        virtual ~PlayerTimePrinter() = default;
        void clearLine();
    private:
        std::string _lastOutput;
    };
}
