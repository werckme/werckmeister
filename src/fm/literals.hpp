#ifndef FM_LITERALS_HPP
#define FM_LITERALS_HPP

#include "units.hpp"

namespace fm {
    Ticks operator "" _N1(long double);
    Ticks operator "" _N2(long double);
    Ticks operator "" _N4(long double);
    Ticks operator "" _N8(long double);
    Ticks operator "" _N16(long double);
    Ticks operator "" _N32(long double);
    Ticks operator "" _N64(long double);
    Ticks operator "" _N128(long double);

    Ticks operator "" _N1p(long double);
    Ticks operator "" _N2p(long double);
    Ticks operator "" _N4p(long double);
    Ticks operator "" _N8p(long double);
    Ticks operator "" _N16p(long double);
    Ticks operator "" _N32p(long double);
    Ticks operator "" _N64p(long double);
    Ticks operator "" _N128p(long double);

    Ticks operator "" _N1t(long double);
    Ticks operator "" _N2t(long double);
    Ticks operator "" _N4t(long double);
    Ticks operator "" _N8t(long double);
    Ticks operator "" _N16t(long double);
    Ticks operator "" _N32t(long double);
    Ticks operator "" _N64t(long double);
    Ticks operator "" _N128t(long double);
}

#endif