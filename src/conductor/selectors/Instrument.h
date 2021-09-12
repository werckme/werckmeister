#ifndef SEL_INSTRUMENT_H
#define SEL_INSTRUMENT_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="instrument">
        ///     Selects any event which belongs to the given instrument.
        /// ## example, select the events after time >= 8 quarters: 
        /// ```
        /// instrument(bass) {...}
        /// ```
        /// <param name="instrument name" type="string"></param>
        /// </selector>
        class Instrument : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~Instrument() = default;
        };
    }
}

#endif