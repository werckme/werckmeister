
#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="all">
    ///     Selects all events.
    /// ```\n
    /// all() {...}\n
    /// ```
    /// </selector>
    class All : public ISelector
    {
    public:
        typedef ASelectorBase Base;
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override
        {
            return true;
        }
        virtual ~All() = default;
    };
}
