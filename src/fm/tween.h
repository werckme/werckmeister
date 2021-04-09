#ifndef FM_TWEEN_H
#define FM_TWEEN_H

#include <cmath>
#include "TemplateMetaHelper.h"

namespace fm {
    enum TweenType { TweenTypeLin, TweenTypeQuad, TweenTypeCub, TweenTypeQuart, TweenTypeQuint, TweenTypeExp };
    /*
        Robert Penner: Motion, Tweening, and Easing.
        http://www.robertpenner.com/easing/penner_chapter7_tweening.pdf
    */
    template <typename T, TweenType TTweenType>
    class Tween  {
    public:
    private:
        T p = T(0);
        T b = T(0);
        T e = T(0);
        T d = T(0);
        T c = T(0);
    public:
        Tween(T b, T e, T d) : b(b), e(e), d(d), c(e - b) {}
        T calc(T t) {
            return calc(t, Int2Type<TTweenType>());
	    }
        T calc(T t, Int2Type<TweenTypeLin>){ return c*t/d + b; }
        T calc(T t, Int2Type<TweenTypeQuad>){ p=t/d; return c*p*p + b; }
        T calc(T t, Int2Type<TweenTypeCub>){ p=t/d; return c*p*p*p + b;  }
        T calc(T t, Int2Type<TweenTypeQuart>){ p=t/d; return c*p*p*p*p + b; }
        T calc(T t, Int2Type<TweenTypeQuint>){ p=t/d; return c*p*p*p*p*p + b; }
        T calc(T t, Int2Type<TweenTypeExp>){ p=t/d; return c * std::pow(T(2.0), T(10.0) * ( p - T(1.0) )) + b; }
    };
}

#endif