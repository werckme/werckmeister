#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="fromPosition">
    ///     Selects any event where its position is after a given time.
    /// see also: [toPosition](#toPosition).
    /// ## example, select the events after time >= 8 quarters:
    /// ```
    /// fromPosition(8) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters"></param>
    class FromPosition : public ISelector
    {
    public:
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~FromPosition() = default;
    };
}
