#ifndef SEL_FROM_POSIITON_H
#define SEL_FROM_POSIITON_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        class FromPosition : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromPosition() = default;
        };
    }
}

#endif