#include "NthBar.h"
#include <compiler/error.hpp>

namespace conductor
{
    bool NthBar::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventInformation = _eventInformationServer->find(ev);
        auto bar = eventInformation->barNumber;		
        for (const auto &argument : arguments)
        {
            int valueToMatch = int(argument.numberValue);
            if (valueToMatch <= 1)
            {
                FM_THROW(compiler::Exception, "nth bar value \"" + std::to_string(valueToMatch) + "\" can not be less than 2");
            }
            if (bar % (valueToMatch) == 0)
            {
                return true;
            }
        }
        return false;
    }
}
