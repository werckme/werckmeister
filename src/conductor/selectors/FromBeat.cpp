#include "FromBeat.h"
#include <cmath>

namespace sheet
{
    namespace conductor
    {
        bool FromBeat::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const
        {
            const auto& ev = *evm.noteOn;
            for (const auto& argument : arguments) {
                auto quarters = ev.absPosition() / fm::PPQ;
                double nominator = double(evm.timeSignature.first);
                double denominator = double(evm.timeSignature.second);
                auto beat = ::fmod(quarters, nominator / denominator * 4);
                auto valueToMatch = argument.numberValue - 1;
                if (beat >= valueToMatch) {
                    return true;
                }
            }
            return false;
        }
    }
}
