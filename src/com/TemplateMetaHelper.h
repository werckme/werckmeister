#ifndef FM_TEMPLATE_HELPER_H
#define FM_TEMPLATE_HELPER_H

namespace com {
    template <int TValue>
    struct Int2Type {
        enum { Value = TValue };
    };
}

#endif