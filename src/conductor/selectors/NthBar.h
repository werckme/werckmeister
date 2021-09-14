#ifndef SEL_NTH_BAR
#define SEL_NTH_BAR

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        /// <selector name="nthBar">
        ///     Selects an event which is located within in one of the give n-th bar
        /// ```
        /// nthBar(1 3 5) {...}
        /// ```
        /// <param name="beat" type="quarters+"></param>
        /// </selector>
        class NthBar : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~NthBar() = default;
        };
    }
}

#endif