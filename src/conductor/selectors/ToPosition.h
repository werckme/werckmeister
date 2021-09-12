#ifndef SEL_TO_POSIITON_H
#define SEL_TO_POSIITON_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="toPosition">
        ///     Selects any event where its position is after a given time.
        /// see also: [toPosition](#toPosition).
        /// ## example, select the events before time <= 8 quarters: 
        /// ```
        /// toPosition(8) {...}
        /// ```
        /// <param name="beat" type="quarters"></param>
        /// </selector>
        class ToPosition : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~ToPosition() = default;
        };
    }
}

#endif