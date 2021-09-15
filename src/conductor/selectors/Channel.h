#ifndef SEL_CHANNEL_H
#define SEL_CHANNEL_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="channel">
        ///     Selects an event where its channel is equal to one the given numbers.
        /// ```
        /// channel(0 1 2) {...}
        /// ```
        /// </selector>
        /// <param name="channel" type="midi channel+"></param>
        class Channel : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~Channel() = default;
        };
    }
}

#endif