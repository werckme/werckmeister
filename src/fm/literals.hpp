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

    Ticks operator "" _N1n5(long double);
    Ticks operator "" _N2n5(long double);
    Ticks operator "" _N4n5(long double);
    Ticks operator "" _N8n5(long double);
    Ticks operator "" _N16n5(long double);
    Ticks operator "" _N32n5(long double);
    Ticks operator "" _N64n5(long double);
    Ticks operator "" _N128n5(long double);

    Ticks operator "" _N1n7(long double);
    Ticks operator "" _N2n7(long double);
    Ticks operator "" _N4n7(long double);
    Ticks operator "" _N8n7(long double);
    Ticks operator "" _N16n7(long double);
    Ticks operator "" _N32n7(long double);
    Ticks operator "" _N64n7(long double);
    Ticks operator "" _N128n7(long double);

    Ticks operator "" _N1n9(long double);
    Ticks operator "" _N2n9(long double);
    Ticks operator "" _N4n9(long double);
    Ticks operator "" _N8n9(long double);
    Ticks operator "" _N16n9(long double);
    Ticks operator "" _N32n9(long double);
    Ticks operator "" _N64n9(long double);
    Ticks operator "" _N128n9(long double);
}

#endif