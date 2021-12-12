#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="toBar">
    ///     Selects an event where its bar time is equal or less.
    /// ```
    /// toBar(4) {...}
    /// ```
    /// see also: [fromBar](#toBar), [onBar](#onBar), [nthBar](#nthBar).
    /// </selector>
    /// <param name="bar" type="bar number"></param>
    class ToBar : public ISelector
    {
    public:
        virtual bool isMatch(const ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToBar() = default;
    };
}
