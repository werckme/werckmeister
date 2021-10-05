#ifndef SEL_FROM_POSIITON_H
#define SEL_FROM_POSIITON_H

#include "ISelector.h"

namespace sheet
{
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
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromPosition() = default;
        };
    }
}

#endif