#ifndef SEL_FROMBEAT_H
#define SEL_FROMBEAT_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="fromBeat">
        ///     Selects an event where its beat time is equal or greater.
        /// ```
        /// fromBeat(1) {...}
        /// ```
        /// </selector>
        /// <param name="beat" type="quarters"></param>
        class FromBeat : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromBeat() = default;
        };
    }
}

#endif