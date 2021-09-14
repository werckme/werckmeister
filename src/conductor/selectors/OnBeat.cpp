#include "OnBeat.h"
#include <cmath>

namespace {
    const fm::Ticks Tolerance = 0.0001;
}

namespace sheet
{
    namespace conductor
    {
        bool OnBeat::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto &argument : arguments) {
                auto quarters = ev.absPosition() / fm::PPQ;
                double nominator = double(evm.timeSignature.first);
                double denominator = double(evm.timeSignature.second);
                auto beat = ::fmod(quarters, nominator/denominator * 4);
                auto valueToMatch = argument.tickValue - 1;
                if (::abs(beat - valueToMatch) <= Tolerance) {
                    return true;
                }
            }
            return false;
        }
    }
}
