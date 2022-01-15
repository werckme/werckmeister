
#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="withTag">
    ///     Selects all events.
    /// ## example, select the event with the tag "myTag" assigned:
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
