#ifndef DECLARATION_LENGTH_H
#define DECLARATION_LENGTH_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class Length : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~Length() = default;
        };
    }
}

#endif