#pragma once

#include "ASelectorBase.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="atPosition">
    ///     Selects any event where its position is equal to a given value.
    /// &gt; Please mind the [cue](#cue) behaviour in combination with repeat and jumps.
    /// </selector>
    /// <param name="position" type="quarters|cuename"></param>
    class AtPosition : public ASelectorBase
    {
    public:
        typedef ASelectorBase Base;
        AtPosition(compiler::IEventInformationServerPtr eventInformationServer) : Base(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~AtPosition() = default;
    };
}
