#include "ToPosition.h"
#include <cmath>
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        bool ToPosition::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const
        {
            const auto& ev = *evm.noteOn;
            if (arguments.empty()) 
            {
                FM_THROW(compiler::Exception, "missing argument for selector ToPosition");
            }
            auto eventPosition = ev.absPosition() / fm::PPQ;
            auto value = arguments[0].numberValue;
            return eventPosition <= value;
        }
    }
}
