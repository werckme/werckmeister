#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// TODO
    class Cc : public ADeclaration
    {
    public:
        virtual void perform(const Events &events) const override;
        virtual ~Cc() = default;
    };
}
