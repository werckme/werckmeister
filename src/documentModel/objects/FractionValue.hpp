#pragma once

#include <com/units.hpp>

namespace documentModel
{
    struct FractionValue
    {
        int nominator = 1;
        int denominator = 1;
        inline operator com::Ticks() const { return (com::Ticks)nominator / (com::Ticks)denominator; }
    };
}
