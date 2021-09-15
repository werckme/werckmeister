#ifndef SEL_ATBEAT_H
#define SEL_ATBEAT_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="atBeat">
        ///     Selects an event at a given beat or a list of beats
        /// ## example, select the events at beat 1 and 3: 
        /// ```
        /// atBeat(1 3) {...}
        /// ```
        /// </selector>
        /// <param name="beat" type="quarters+"></param>
        class OnBeat : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~OnBeat() = default;
        };
    }
}

#endif