#pragma once

#include <compiler/Instrument.h>

namespace compiler
{
    class ICanSpecifyInstrument
    {
    public:
        virtual ~ICanSpecifyInstrument() = default;
        virtual void setInstrument(AInstrumentDefPtr instrument) = 0;
        virtual AInstrumentDefPtr getInstrument() = 0;
        virtual bool hasInstrument() const = 0;
    };
}
