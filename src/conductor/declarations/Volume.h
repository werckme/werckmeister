#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// TODO
    class Volume : public ADeclaration
    {
    public:
        virtual void perform(const Events &events) const override;
        virtual ~Volume() = default;
    };
}
