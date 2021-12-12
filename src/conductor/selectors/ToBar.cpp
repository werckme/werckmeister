#include "ToBar.h"

namespace conductor
{
    bool ToBar::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        for (const auto &argument : arguments)
        {
            int valueToMatch = int(argument.numberValue - 1);
            if (int(evm.barNumber) <= valueToMatch)
            {
                return true;
            }
        }
        return false;
    }
}
