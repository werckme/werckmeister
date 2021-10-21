#include "NotOnBeat.h"
#include <cmath>

namespace {
    const fm::Ticks Tolerance = 0.0001;
}

namespace sheet
{
    namespace conductor
    {
        bool NotOnBeat::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto &argument : arguments) {
                auto quarters = ev.absPosition() / fm::PPQ;
                auto nominator = fm::Ticks(evm.timeSignature.first);
                auto denominator = fm::Ticks(evm.timeSignature.second);
                auto beat = ::fmod(quarters, nominator/denominator * fm::Ticks(4.0));
                auto valueToMatch = argument.numberValue - 1;
                if (::fabs(beat - valueToMatch) <= Tolerance) {
                    return false;
                }
            }
            return true;
        }
    }
}
