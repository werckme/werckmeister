#include "FadeCC.h"
#include <compiler/context/IContext.h>
#include <com/werckmeister.hpp>
#include <algorithm>
#include <com/tween.h>
#include <compiler/error.hpp>

namespace compiler
{
    namespace
    {
        template <class TTween>
        void _perform(const TTween &tween, com::Ticks duration, IContextPtr context, int controllerNr)
        {
            using namespace com;
            for (double t = 0; t <= duration; t += double(1.0_N128))
            {
                auto doubleValue = tween.calc(t);
                int value = std::max(0, std::min(127, int(doubleValue)));
                context->setContinuousController(controllerNr, value, com::Ticks(t));
            }
        }
    }
    void FadeCC::execute(IContextPtr context)
    {
        int nr = getControllerNr();
        auto from = parameters[argumentNames.FadeCC.From].value<double>();
        auto to = parameters[argumentNames.FadeCC.To].value<double>();
        auto duration = parameters[argumentNames.FadeCC.Duration].value<double>();
        auto curveType = parameters[argumentNames.FadeCC.Curve].value<com::String>();
        duration *= com::PPQ;
        if (curveType == SetFade::CurveTypeLin)
        {
            com::Tween<double, com::TweenTypeLin> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else if (curveType == SetFade::CurveTypeQuad)
        {
            com::Tween<double, com::TweenTypeQuad> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else if (curveType == SetFade::CurveTypeCub)
        {
            com::Tween<double, com::TweenTypeCub> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else if (curveType == SetFade::CurveTypeQuart)
        {
            com::Tween<double, com::TweenTypeQuart> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else if (curveType == SetFade::CurveTypeQuint)
        {
            com::Tween<double, com::TweenTypeQuint> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else if (curveType == SetFade::CurveTypeExp)
        {
            com::Tween<double, com::TweenTypeExp> tween(from, to, duration);
            _perform(tween, duration, context, nr);
        }
        else
        {
            FM_THROW(Exception, "unsupported curve type: " + curveType);
        }
    }
}