#include "OnBar.h"



namespace sheet
{
    namespace conductor
    {
        bool OnBar::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto& argument : arguments) {
                auto quarters = ev.absPosition() / fm::PPQ;
                double nominator = double(evm.timeSignature.first);
                double denominator = double(evm.timeSignature.second);
                int bar = int((quarters * denominator) / (4.0 * nominator));
                int valueToMatch = int(argument.tickValue - 1);
                if (valueToMatch == bar) {
                    return true;
                }
            }
            return false;
        }
    }
}
