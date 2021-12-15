#include "ToPosition.h"
#include <cmath>
#include <compiler/error.hpp>

namespace conductor
{
    bool ToPosition::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        if (arguments.empty())
        {
            FM_THROW(compiler::Exception, "missing argument for selector ToPosition");
        }
        auto argument = arguments[0];
        auto value = argument.numberValue;
        if (argument.valueContext == documentModel::ConductionSelector::ArgumentValue::CueReference)
        {
            auto cueInfos = _eventInformationServer->findCueEvents(argument.name);
            if (!cueInfos.empty())
            {
                value = cueInfos.begin()->absolutePosition / com::PPQ;
            }
        }
        auto eventPosition = ev.absPosition() / com::PPQ;
        return eventPosition < value;
    }
}
