#ifndef SEL_FROMBAR_H
#define SEL_FROMBAR_H

#include "ISelector.h"

namespace documentModel
{
    namespace conductor
    {
        /// <selector name="fromBar">
        ///     Selects an event where its bar time is equal or greater.
        /// ```
        /// fromBar(1) {...}
        /// ```
        /// see also: [toBar](#toBar), [onBar](#onBar), [nthBar](#nthBar).
        /// </selector>
        /// <param name="bar" type="bar number"></param>
        class FromBar : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromBar() = default;
        };
    }
}

#endif