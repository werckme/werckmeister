#include "SetFade.h"
#include <compiler/context/IContext.h>
#include <com/literals.hpp>
#include <compiler/error.hpp>

namespace compiler
{
    void SetFade::execute(IContextPtr context)
    {
        auto from = parameters[argumentNames.SetFade.FadeFrom].value<double>();
        auto to = parameters[argumentNames.SetFade.FadeTo].value<double>() / 100.0 * 127.0;
        auto duration = parameters[argumentNames.SetFade.FadeDuration].value<double>();
        auto curveType = parameters[argumentNames.SetFade.Curve].value<com::String>();
        Base::fade(context, 11, from, to, duration, curveType);
    }

}