#ifndef SEL_TOBEAT_H
#define SEL_TOBEAT_H

#include "ISelector.h"

namespace sheet
{
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
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~ToBeat() = default;
        };
    }
}

#endif