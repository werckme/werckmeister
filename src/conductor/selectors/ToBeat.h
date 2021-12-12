#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="toBeat">
    /// Selects an event where its beat time is equal or less.
    /// see also: [fromBeat](#fromBeat), [onBeat](#onBeat).
    /// ```
    /// toBeat(4) {...}
    /// ```
    /// </selector>
    /// <param name="beat" type="quarters"></param>
    class ToBeat : public ISelector
    {
    public:
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToBeat() = default;
    };
}
