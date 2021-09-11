#ifndef DECLARATION_LENGTH_H
#define DECLARATION_LENGTH_H

#include "IDeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class Length : public IDeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~Length() = default;
        };
    }
}

#endif