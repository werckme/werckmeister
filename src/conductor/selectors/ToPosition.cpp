#include "ToPosition.h"
#include <cmath>
#include <compiler/error.hpp>

namespace conductor
{
    bool ToPosition::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventPosition = ev.absPosition() / com::PPQ;
        if (arguments.empty())
        {
            FM_THROW(compiler::Exception, "missing argument for selector ToPosition");
        }
        auto argument = arguments[0];
        if (argument.valueContext != documentModel::ConductionSelector::ArgumentValue::CueReference)
        {
            return eventPosition < argument.numberValue;
        }
        auto cuePosition = _eventInformationServer->findCueEventPosition(argument.name);
        if (cuePosition == UndefinedTicks)
        {
            return false;
        }
        return eventPosition < cuePosition/com::PPQ;
    }
}
