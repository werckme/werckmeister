#include "FromPosition.h"
#include <compiler/error.hpp>

namespace conductor
{
    bool FromPosition::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
        auto eventPosition = ev.absPosition() / com::PPQ;
        if (arguments.empty())
        {
            FM_THROW(compiler::Exception, "missing argument for selector FromPosition");
        }
        auto argument = arguments[0];
        if (argument.valueContext != documentModel::ConductionSelector::ArgumentValue::CueReference)
        {
            return eventPosition >= argument.numberValue;
        }
        auto loopIndex = getLoopIndex(evm);
        auto cueInfos = _eventInformationServer->findCueEvents(argument.name);
        if (cueInfos.empty())
        {
            return false;
        }
        const auto &cueInfo = *cueInfos.begin();
        const auto &cueInfoPositions = cueInfo.positions;
        if (loopIndex >= cueInfoPositions.size()) 
        {
            return false;
        }
        const auto cuePosition = cueInfoPositions.at(loopIndex);
        return eventPosition >= cuePosition/com::PPQ;
    }
}