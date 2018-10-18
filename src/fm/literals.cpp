#include "literals.hpp"
#include "config.hpp"
#include <math.h>

namespace fm {
    Ticks operator "" _N1(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 4.0) );
    }
    Ticks operator "" _N2(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 2.0) );
    }
    Ticks operator "" _N4(long double val)
    {
        return static_cast<Ticks>( floor(val * PPQ) );
    }
    Ticks operator "" _N8(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 2) );
    }
    Ticks operator "" _N16(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 4) );
    }
    Ticks operator "" _N32(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 8) );
    }
    Ticks operator "" _N64(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 16) );
    }
    Ticks operator "" _N128(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 32) );
    }


    Ticks operator "" _N1p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 4.0 + PPQ * 2.0) );
    }
    Ticks operator "" _N2p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 2.0 + PPQ) );
    }
    Ticks operator "" _N4p(long double val)
    {
        return static_cast<Ticks>( floor(val * PPQ + PPQ / 2) );
    }
    Ticks operator "" _N8p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 2 + PPQ / 4) );
    }
    Ticks operator "" _N16p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 4 + PPQ / 8) );
    }
    Ticks operator "" _N32p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 8 + PPQ / 16) );
    }
    Ticks operator "" _N64p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 16 + PPQ / 32) );
    }
    Ticks operator "" _N128p(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 32 + PPQ / 64) );
    }

    
    Ticks operator "" _N1t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 8.0 / 3.0) );
    }
    Ticks operator "" _N2t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ * 4.0 / 3.0) );
    }
    Ticks operator "" _N4t(long double val)
    {
        return static_cast<Ticks>( floor(val * PPQ * 2.0 / 3.0) );
    }
    Ticks operator "" _N8t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 3.0) );
    }
    Ticks operator "" _N16t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 2.0 / 3.0) );
    }
    Ticks operator "" _N32t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 4.0 / 3.0) );
    }
    Ticks operator "" _N64t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 8.0 / 3.0) );
    }
    Ticks operator "" _N128t(long double val) 
    {
        return static_cast<Ticks>( floor(val * PPQ / 16.0 / 3.0 ) );
    }
}