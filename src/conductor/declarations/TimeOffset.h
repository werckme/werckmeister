#ifndef DECLARATION_TIME_H
#define DECLARATION_TIME_H

#include "IDeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class TimeOffset : public IDeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~TimeOffset() = default;
        };
    }
}

#endif