#ifndef SEL_FROMBAR_H
#define SEL_FROMBAR_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="fromBar">
        ///     Selects an event where its bar time is equal or greater 
        /// ```
        /// fromBar(1) {...}
        /// ```
        /// <param name="bar" type="bar number"></param>
        /// </selector>
        class FromBar : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromBar() = default;
        };
    }
}

#endif