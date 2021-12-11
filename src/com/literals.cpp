#include "literals.hpp"
#include "config.hpp"
#include <math.h>

namespace com {
    Ticks operator "" _N1(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0) );
    }
    Ticks operator "" _N2(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 2.0) );
    }
    Ticks operator "" _N4(long double val)
    {
        return static_cast<Ticks>( (val * PPQ) );
    }
    Ticks operator "" _N8(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 2) );
    }
    Ticks operator "" _N16(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 4) );
    }
    Ticks operator "" _N32(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 8) );
    }
    Ticks operator "" _N64(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 16) );
    }
    Ticks operator "" _N128(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 32) );
    }


    Ticks operator "" _N1p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 + PPQ * 2.0) );
    }
    Ticks operator "" _N2p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 + PPQ) );
    }
    Ticks operator "" _N4p(long double val)
    {
        return static_cast<Ticks>( (val * PPQ + PPQ/2) );
    }
    Ticks operator "" _N8p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/2 + PPQ/4) );
    }
    Ticks operator "" _N16p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/4 + PPQ/8) );
    }
    Ticks operator "" _N32p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/8 + PPQ/16) );
    }
    Ticks operator "" _N64p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/16 + PPQ/32) );
    }
    Ticks operator "" _N128p(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/32 + PPQ/64) );
    }

    Ticks operator "" _N1pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 + PPQ * 2.0 + PPQ) );
    }
    Ticks operator "" _N2pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 + PPQ + PPQ/2) );
    }
    Ticks operator "" _N4pp(long double val)
    {
        return static_cast<Ticks>( (val * PPQ + PPQ/2 + PPQ/4) );
    }
    Ticks operator "" _N8pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/2 + PPQ/4 + PPQ/8) );
    }
    Ticks operator "" _N16pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/4 + PPQ/8 + PPQ/16) );
    }
    Ticks operator "" _N32pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/8 + PPQ/16 + PPQ/32) );
    }
    Ticks operator "" _N64pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/16 + PPQ/32 + PPQ/64) );
    }
    Ticks operator "" _N128pp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/32 + PPQ/64 + PPQ/128) );
    }

    Ticks operator "" _N1ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 + PPQ * 2.0 + PPQ + PPQ/2) );
    }
    Ticks operator "" _N2ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 + PPQ + PPQ/2 + PPQ/4) );
    }
    Ticks operator "" _N4ppp(long double val)
    {
        return static_cast<Ticks>( (val * PPQ + PPQ/2 + PPQ/4 + PPQ/8) );
    }
    Ticks operator "" _N8ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/2 + PPQ/4 + PPQ/8 + PPQ/16) );
    }
    Ticks operator "" _N16ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/4 + PPQ/8 + PPQ/16 + PPQ/32) );
    }
    Ticks operator "" _N32ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/8 + PPQ/16 + PPQ/32 + PPQ/64) );
    }
    Ticks operator "" _N64ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/16 + PPQ/32 + PPQ/64 + PPQ/128) );
    }
    Ticks operator "" _N128ppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/32 + PPQ/64 + PPQ/128 + PPQ/256) );
    }
    
    Ticks operator "" _N1pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 + PPQ * 2.0 + PPQ + PPQ/2 + PPQ/4) );
    }
    Ticks operator "" _N2pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 + PPQ + PPQ/2 + PPQ/4 + PPQ/8) );
    }
    Ticks operator "" _N4pppp(long double val)
    {
        return static_cast<Ticks>( (val * PPQ + PPQ/2 + PPQ/4 + PPQ/8 + PPQ/16) );
    }
    Ticks operator "" _N8pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/2 + PPQ/4 + PPQ/8 + PPQ/16 + PPQ/32) );
    }
    Ticks operator "" _N16pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/4 + PPQ/8 + PPQ/16 + PPQ/32 + PPQ/64) );
    }
    Ticks operator "" _N32pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/8 + PPQ/16 + PPQ/32 + PPQ/64 + PPQ/128) );
    }
    Ticks operator "" _N64pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/16 + PPQ/32 + PPQ/64 + PPQ/128 + PPQ/256) );
    }
    Ticks operator "" _N128pppp(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ/32 + PPQ/64 + PPQ/128 + PPQ/256 + PPQ/512) );
    }

    Ticks operator "" _N1t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 8.0 / 3.0) );
    }
    Ticks operator "" _N2t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 / 3.0) );
    }
    Ticks operator "" _N4t(long double val)
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 / 3.0) );
    }
    Ticks operator "" _N8t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 3.0) );
    }
    Ticks operator "" _N16t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 2.0 / 3.0) );
    }
    Ticks operator "" _N32t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 4.0 / 3.0) );
    }
    Ticks operator "" _N64t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 8.0 / 3.0) );
    }
    Ticks operator "" _N128t(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 16.0 / 3.0 ) );
    }


    Ticks operator "" _N1n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 8.0 / 5.0) );
    }
    Ticks operator "" _N2n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 / 5.0) );
    }
    Ticks operator "" _N4n5(long double val)
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 / 5.0) );
    }
    Ticks operator "" _N8n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 5.0) );
    }
    Ticks operator "" _N16n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 2.0 / 5.0) );
    }
    Ticks operator "" _N32n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 4.0 / 5.0) );
    }
    Ticks operator "" _N64n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 8.0 / 5.0) );
    }
    Ticks operator "" _N128n5(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 16.0 / 5.0 ) );
    }


    Ticks operator "" _N1n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 8.0 / 7.0) );
    }
    Ticks operator "" _N2n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 / 7.0) );
    }
    Ticks operator "" _N4n7(long double val)
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 / 7.0) );
    }
    Ticks operator "" _N8n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 7.0) );
    }
    Ticks operator "" _N16n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 2.0 / 7.0) );
    }
    Ticks operator "" _N32n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 4.0 / 7.0) );
    }
    Ticks operator "" _N64n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 8.0 / 7.0) );
    }
    Ticks operator "" _N128n7(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 16.0 / 7.0 ) );
    }


    Ticks operator "" _N1n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 8.0 / 9.0) );
    }
    Ticks operator "" _N2n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ * 4.0 / 9.0) );
    }
    Ticks operator "" _N4n9(long double val)
    {
        return static_cast<Ticks>( (val * PPQ * 2.0 / 9.0) );
    }
    Ticks operator "" _N8n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 9.0) );
    }
    Ticks operator "" _N16n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 2.0 / 9.0) );
    }
    Ticks operator "" _N32n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 4.0 / 9.0) );
    }
    Ticks operator "" _N64n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 8.0 / 9.0) );
    }
    Ticks operator "" _N128n9(long double val) 
    {
        return static_cast<Ticks>( (val * PPQ / 16.0 / 9.0 ) );
    }
}