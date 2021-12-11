#include "OnBeat.h"
#include <cmath>

namespace {
    const com::Ticks Tolerance = 0.0001;
}

namespace sheet
{
    namespace conductor
    {
        bool OnBeat::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto &argument : arguments) {
                auto quarters = ev.absPosition() / com::PPQ;
                auto nominator = com::Ticks(evm.timeSignature.first);
                auto denominator = com::Ticks(evm.timeSignature.second);
                auto beat = ::fmod(quarters, nominator/denominator * com::Ticks(4.0));
                auto valueToMatch = argument.numberValue - 1;
                if (::fabs(beat - valueToMatch) <= Tolerance) {
                    return true;
                }
            }
            return false;
        }
    }
}
