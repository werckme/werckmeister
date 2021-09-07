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
            virtual ~AtBeat() = default;
        };
    }
}

#endif