#include "SetFade.h"
#include <compiler/context/IContext.h>
#include <fm/tween.h>
#include <fm/literals.hpp>
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        const fm::String SetFade::CurveTypeLin = "lin";
        const fm::String SetFade::CurveTypeQuad = "quad";
        const fm::String SetFade::CurveTypeCub = "cub";
        const fm::String SetFade::CurveTypeQuart = "quart";
        const fm::String SetFade::CurveTypeQuint = "quint";
        const fm::String SetFade::CurveTypeExp = "exp";
        namespace {
            template<class TTween>
            void _perform(const TTween &tween, fm::Ticks duration, IContextPtr  context)
            {
                using namespace fm;
                for (double t=0; t<=duration; t+= double(1.0_N128) ) {
                    auto value = tween.calc(t);
                    context->setVolume(value, fm::Ticks(t));
                }
            }
        }
        void SetFade::execute(IContextPtr  context)
        {
            auto from = parameters[argumentNames.SetFade.FadeFrom].value<double>();
            auto to = parameters[argumentNames.SetFade.FadeTo].value<double>();
            auto duration = parameters[argumentNames.SetFade.FadeDuration].value<double>();
            auto curveType = parameters[argumentNames.SetFade.Curve].value<fm::String>();
            duration *= fm::PPQ;
            if (curveType == CurveTypeLin)  {
                fm::Tween<double, fm::TweenTypeLin> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuad)  {
                fm::Tween<double, fm::TweenTypeQuad> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeCub)  {
                fm::Tween<double, fm::TweenTypeCub> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuart)  {
                fm::Tween<double, fm::TweenTypeQuart> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuint)  {
                fm::Tween<double, fm::TweenTypeQuint> tween(from, to, duration);
                _perform(tween, duration, context);
            }   
            else if (curveType == CurveTypeExp)  {
                fm::Tween<double, fm::TweenTypeExp> tween(from, to, duration);
                _perform(tween, duration, context);
            } else {
                FM_THROW(Exception, "unsupported curve type: " + curveType); 
            }                                                       
        }

    }
}