#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="nthBar">
    /// Selects an event which is located within in one of the give n-th bar.\n
    /// > Heads Up:  The nth bar is not affected by any other selector.\n
    /// > Here for example: `fromBar(1) nthBar(2) ...`, the nth bar selector\n
    /// will consider its decision counting from bar 0, not from the selected bar 1.\n
    /// ```
    /// nthBar(1 3 5) {...}
    /// ```
    /// see also: [fromBar](#fromBar), [toBar](#toBar), [onBar](#onBar).
    /// </selector>
    /// <param name="beat" type="bar number+"></param>
    class NthBar : public ISelector
    {
    public:
        virtual bool isMatch(const ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~NthBar() = default;
    };
}
