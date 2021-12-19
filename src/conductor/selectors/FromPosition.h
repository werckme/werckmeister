#pragma once

#include "ASelectorBase.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="fromPosition">
    ///     Selects any event where its position is equal or after a given position.
    /// see also: [toPosition](#toPosition).
    /// ## example, select the events postion >=8 quarters:
    /// ```\n
    /// fromPosition(8) {...}\n
    /// ```\n
    /// ## using cue markers instead of quarter numbers
    /// ```\n
    /// c d e f | /cue: myMark/ g a b 'c\n
    /// ```\n
    /// ```\n
    /// fromPosition(@myMark) {...}\n
    /// ```\n
    /// &gt; In order to work properly, a cue name must occur only once in a document.
    /// </selector>
    /// <param name="position" type="quarters|cuename"></param>
    class FromPosition : public ASelectorBase
    {
    public:
        typedef ASelectorBase Base;
        FromPosition(compiler::IEventInformationServerPtr eventInformationServer) : Base(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~FromPosition() = default;
    };
}
