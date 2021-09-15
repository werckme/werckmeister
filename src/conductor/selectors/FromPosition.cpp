#include "FromPosition.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        bool FromPosition::isMatch(const ConductionSelector::Arguments& arguments, const EventWithMetaInfo& evm) const 
        {
            const auto& ev = *evm.noteOn;
            if (arguments.empty()) 
            {
                FM_THROW(compiler::Exception, "missing argument for selector FromPosition");
            }
            auto eventPosition = ev.absPosition() / fm::PPQ;
            auto value = arguments[0].numberValue - 1;
            return eventPosition >= value;
        }
    }
}
