#include "AtBeat.h"
#include <cmath>

namespace {
    const fm::Ticks Tolerance = 0.0001;
}

namespace sheet
{
    namespace conductor
    {
        bool AtBeat::isMatch(const ConductionSelector::Arguments& arguments, const fm::midi::Event& ev) const 
        {
            for (const auto &argument : arguments) {
                auto quarters = ev.absPosition() / fm::PPQ;
                auto beat = ::fmod(quarters, 4); // TODO replace 4 with something dynamic
                auto valueToMatch = argument.tickValue - 1;
                if (::abs(beat - valueToMatch) <= Tolerance) {
                    return true;
                }
            }
            return false;
        }
    }
}
