#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// TODO
    class Bend : public ADeclaration
    {
    public:
        virtual void perform(const Events &events) const override;
        virtual ~Bend() = default;
    };
}
