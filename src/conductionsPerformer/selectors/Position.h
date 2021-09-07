#ifndef SEL_POSITION_H
#define SEL_POSITION_H

#include "ISelector.h"

namespace sheet
{
    namespace conductor
    {
        class Position : public ISelector
        {
        public:
            virtual ~Position() = default;
        };
    }
}

#endif