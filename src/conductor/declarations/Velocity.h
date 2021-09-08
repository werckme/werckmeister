#ifndef DECLARATION_VELOCITY_H
#define DECLARATION_VELOCITY_H

#include "IDeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class Velocity : public IDeclaration
        {
        public:
            virtual void perform(fm::midi::Event&) const override;
            virtual ~Velocity() = default;
        };
    }
}

#endif