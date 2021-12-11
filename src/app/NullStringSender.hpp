#pragma once

#include "IStringSender.hpp"

namespace app  {
    class NullStringSender : public IStringSender {
    public:
        virtual void send(const char *bytes, size_t length) override {};
        virtual ~NullStringSender() = default;
    };
}

