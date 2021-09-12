#ifndef DECLARATION_VELOCITY_H
#define DECLARATION_VELOCITY_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class Velocity : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~Velocity() = default;
        };
    }
}

#endif