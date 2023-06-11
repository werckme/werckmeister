#include "FadeCC.h"
#include <compiler/context/IContext.h>
#include <com/werckmeister.hpp>
#include <algorithm>
#include <com/tween.h>
#include <compiler/error.hpp>

namespace compiler
{
    const com::String FadeCC::CurveTypeLin = "lin";
    const com::String FadeCC::CurveTypeQuad = "quad";
    const com::String FadeCC::CurveTypeCub = "cub";
    const com::String FadeCC::CurveTypeQuart = "quart";
    const com::String FadeCC::CurveTypeQuint = "quint";
    const com::String FadeCC::CurveTypeExp = "exp";
    namespace
    {
        template <class TTween>
        void _perform(IContextPtr context, FadeCC* caller, const TTween &tween, com::Ticks duration, int controllerNr)
        {
            using namespace com;
            for (double t = 0; t <= duration; t += double(1.0_N128))
            {
                auto doubleValue = tween.calc(t);
                int value = std::max(0, std::min(127, int(doubleValue)));
                caller->renderController(context, controllerNr, value, com::Ticks(t));
            }
        }
    }

    void FadeCC::fade(IContextPtr context, int nr, double from, double to, double duration, const com::String& curveType)
    {
        duration *= com::PPQ;
        if (curveType == FadeCC::CurveTypeLin)
        {
            com::Tween<double, com::TweenTypeLin> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else if (curveType == FadeCC::CurveTypeQuad)
        {
            com::Tween<double, com::TweenTypeQuad> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else if (curveType == FadeCC::CurveTypeCub)
        {
            com::Tween<double, com::TweenTypeCub> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else if (curveType == FadeCC::CurveTypeQuart)
        {
            com::Tween<double, com::TweenTypeQuart> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else if (curveType == FadeCC::CurveTypeQuint)
        {
            com::Tween<double, com::TweenTypeQuint> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else if (curveType == FadeCC::CurveTypeExp)
        {
            com::Tween<double, com::TweenTypeExp> tween(from, to, duration);
            _perform(context, this, tween, duration, nr);
        }
        else
        {
            FM_THROW(Exception, "unsupported curve type: " + curveType);
        }
    }

    void FadeCC::execute(IContextPtr context)
    {
        int nr = getControllerNr();
        auto from = parameters[argumentNames.FadeCC.From].value<double>();
        auto to = parameters[argumentNames.FadeCC.To].value<double>();
        auto duration = parameters[argumentNames.FadeCC.Duration].value<double>();
        auto curveType = parameters[argumentNames.FadeCC.Curve].value<com::String>();
        fade(context, nr, from, to, duration, curveType);
    }
}