#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="cc">
    ///     Matches a MIDI event to one or more given cc numbers.
    ///     &gt; Does not work in combination with a lot of other selectors such as instrument, chord, expression ...
    ///     &gt; Works with channel, atPosition, fromPosition, toPosition
    /// ```
    /// cc(1 2 3) {...}
    /// ```
    /// </selector>
    /// <param name="cc number" type="1..127"></param>
    class Cc : public ISelector
    {
    public:
        Cc(){}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Cc() = default;

    private:
    };
}
