#pragma once

#include "ICanSpecifyInstrument.h"

namespace compiler
{
    class ACanSpecifyInstrument : public ICanSpecifyInstrument
    {
    public:
        virtual ~ACanSpecifyInstrument() = default;
        virtual void setInstrument(AInstrumentDefPtr instrument) override { this->_instr = instrument; }
        virtual AInstrumentDefPtr getInstrument() override { return this->_instr; }
        virtual bool hasInstrument() const override { return this->_instr != nullptr; }

    private:
        AInstrumentDefPtr _instr = nullptr;
    };
}
