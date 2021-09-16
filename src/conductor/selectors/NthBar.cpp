#include "NthBar.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        bool NthBar::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            for (const auto& argument : arguments) {
                int valueToMatch = int(argument.numberValue);
                if (valueToMatch <= 1)
                {
                    FM_THROW(compiler::Exception, "nth bar value \"" + std::to_string(valueToMatch) + "\" can not be less than 2");
                }
                if (int(evm.barNumber) % (valueToMatch) == 0) {
                    return true;
                }
            }
            return false;
        }
    }
}
