#pragma once

#include "IStringSender.hpp"

namespace app
{
    class NullStringSender : public IStringSender
    {
    public:
        virtual void send(const char *, size_t) override{};
        virtual ~NullStringSender() = default;
    };
}
