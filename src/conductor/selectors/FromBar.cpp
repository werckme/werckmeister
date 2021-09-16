#include "FromBar.h"

namespace sheet
{
    namespace conductor
    {
        bool FromBar::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            for (const auto& argument : arguments) {
                int valueToMatch = int(argument.numberValue - 1);
                if (int(evm.barNumber) >= valueToMatch) {
                    return true;
                }
            }
            return false;
        }
    }
}
