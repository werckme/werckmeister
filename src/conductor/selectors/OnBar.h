#ifndef SEL_ONBAR_H
#define SEL_ONBAR_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="onBar">
        ///     Selects an event on a given bar number or a list of bar numbers
        /// ```
        /// onBar(1 3) {...}
        /// ```
        /// see also: [fromBar](#fromBar), [toBar](#toBar), [nthBar](#nthBar).
        /// </selector>
        /// <param name="beat" type="bar number+"></param>
        class OnBar : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~OnBar() = default;
        };
    }
}

#endif