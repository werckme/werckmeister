#ifndef SEL_NOT_ON_BEAT_H
#define SEL_NOT_ON_BEAT_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="notOnBeat">
        ///     Selects an event which does not appear on a given beat or a list of beats
        /// ```
        /// notOnBeat(1 3) {...}
        /// ```
        /// </selector>
        /// <param name="beat" type="quarters+"></param>
        class NotOnBeat : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~NotOnBeat() = default;
        };
    }
}

#endif