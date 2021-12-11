#pragma once

#include <string>
#include <memory>

namespace app  {
    class IStringSender {
    public:
        virtual void send(const char *bytes, size_t length) = 0;
        virtual ~IStringSender() = default;
    };
    typedef std::shared_ptr<IStringSender> IStringSenderPtr;
}

