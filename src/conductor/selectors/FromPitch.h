#ifndef SEL_FROM_PITCH_H
#define SEL_FROM_PITCH_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="fromPitch">
        ///     Selects any event where its pitch is higher or equal than the given pitch.
        /// see also: [toPitch](#toPitch), [pitch](#pitch).
        /// ```
        /// fromPitch(c,) {...}
        /// ```
        /// <param name="pitch" type="pitch"></param>
        /// </selector>
        class FromPitch : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromPitch() = default;
        };
    }
}

#endif