#include "FromPosition.h"
#include <compiler/error.hpp>

namespace conductor
{
    bool FromPosition::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        if (arguments.empty())
        {
            FM_THROW(compiler::Exception, "missing argument for selector FromPosition");
        }
        auto eventPosition = ev.absPosition() / com::PPQ;
        auto value = arguments[0].numberValue;
        return eventPosition >= value;
    }
}