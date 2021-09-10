#ifndef SEL_TO_POSIITON_H
#define SEL_TO_POSIITON_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        class ToPosition : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const fm::midi::Event&) const override;
            virtual ~ToPosition() = default;
        };
    }
}

#endif