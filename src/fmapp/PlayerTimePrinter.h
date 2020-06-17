#ifndef FMAPP_PLAYERTIMEPRINTER_HPP
#define FMAPP_PLAYERTIMEPRINTER_HPP


#include "IPlayerLoopVisitor.h"

namespace fmapp {
    class PlayerTimePrinter : public IPlayerLoopVisitor {
    public:
        virtual void visit(fm::Ticks elapsed) override;  
        virtual ~PlayerTimePrinter() = default;
    private:
        std::string _lastOutput;
    };
}
#endif