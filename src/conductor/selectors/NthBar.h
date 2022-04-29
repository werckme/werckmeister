#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="nthBar">
    /// Selects the events of every nth bar, counting from the first bar.\n
    /// ```
    /// nthBar(3 5) {...}
    /// ```
    /// see also: [fromBar](#fromBar), [toBar](#toBar), [onBar](#onBar).
    /// </selector>
    /// <param name="beat" type="bar number+"></param>
    class NthBar : public ISelector
    {
    public:
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~NthBar() = default;
    };
}
