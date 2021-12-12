#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="toPosition">
    ///     Selects any event where its position is after a given time.
    /// see also: [toPosition](#toPosition).
    /// ## example, select the events before time &lt;= 8 quarters:
    /// ```
    /// toPosition(8) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters"></param>
    class ToPosition : public ISelector
    {
    public:
        virtual bool isMatch(const ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToPosition() = default;
    };
}
