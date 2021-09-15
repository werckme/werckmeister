#include "ToBar.h"

namespace sheet
{
    namespace conductor
    {
        bool ToBar::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            for (const auto& argument : arguments) {
                int valueToMatch = int(argument.tickValue - 1);
                if (int(evm.barNumber) <= valueToMatch) {
                    return true;
                }
            }
            return false;
        }
    }
}
