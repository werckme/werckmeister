#ifndef FMAPP_PLAYERTIMEPRINTER_HPP
#define FMAPP_PLAYERTIMEPRINTER_HPP


#include "IPlayerLoopVisitor.h"

namespace fmapp {
    class PlayerTimePrinter : public IPlayerLoopVisitor {
    public:
        virtual void loopBegin() override {}
        virtual void loopEnd() override;
        virtual void visit(fm::Ticks elapsed) override;  
        virtual ~PlayerTimePrinter() = default;
        void clearLine();
    private:
        std::string _lastOutput;
    };
}
#endif