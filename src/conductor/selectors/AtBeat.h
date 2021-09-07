#ifndef SEL_ATBEAT_H
#define SEL_ATBEAT_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        class AtBeat : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const fm::midi::Event&) const override
            {
                return false;
            }
            virtual ~AtBeat() = default;
        };
    }
}

#endif