#pragma once

namespace com
{
    template <int TValue>
    struct Int2Type
    {
        enum
        {
            Value = TValue
        };
    };
}
