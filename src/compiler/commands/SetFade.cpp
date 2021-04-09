#include "SetFade.h"
#include <compiler/context/IContext.h>
#include <fm/tween.h>
#include <fm/literals.hpp>

namespace sheet {
    namespace compiler {
        void SetFade::execute(IContextPtr  context)
        {
            using namespace fm;
            auto from = parameters[argumentNames.SetFade.FadeFrom].value<double>();
            auto to = parameters[argumentNames.SetFade.FadeTo].value<double>();
            auto duration = parameters[argumentNames.SetFade.FadeDuration].value<double>();
            duration *= fm::PPQ;
            fm::Tween<double, fm::TweenTypeLin> tween(from, to, duration);
            for (double t=0; t<=duration; t+= double(1.0_N128) ) {
                auto value = tween.calc(t);
                context->setVolume(value, fm::Ticks(t));
            }
        }

    }
}