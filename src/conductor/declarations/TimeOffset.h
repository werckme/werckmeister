#ifndef DECLARATION_TIME_H
#define DECLARATION_TIME_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class TimeOffset : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~TimeOffset() = default;
        };
    }
}

#endif