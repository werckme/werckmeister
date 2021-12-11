#include "SetFade.h"
#include <compiler/context/IContext.h>
#include <com/tween.h>
#include <com/literals.hpp>
#include <compiler/error.hpp>

namespace documentModel {
    namespace compiler {
        const com::String SetFade::CurveTypeLin = "lin";
        const com::String SetFade::CurveTypeQuad = "quad";
        const com::String SetFade::CurveTypeCub = "cub";
        const com::String SetFade::CurveTypeQuart = "quart";
        const com::String SetFade::CurveTypeQuint = "quint";
        const com::String SetFade::CurveTypeExp = "exp";
        namespace {
            template<class TTween>
            void _perform(const TTween &tween, com::Ticks duration, IContextPtr  context)
            {
                using namespace com;
                for (double t=0; t<=duration; t+= double(1.0_N128) ) {
                    auto value = tween.calc(t);
                    context->setVolume(value, com::Ticks(t));
                }
            }
        }
        void SetFade::execute(IContextPtr  context)
        {
            auto from = parameters[argumentNames.SetFade.FadeFrom].value<double>();
            auto to = parameters[argumentNames.SetFade.FadeTo].value<double>();
            auto duration = parameters[argumentNames.SetFade.FadeDuration].value<double>();
            auto curveType = parameters[argumentNames.SetFade.Curve].value<com::String>();
            duration *= com::PPQ;
            if (curveType == CurveTypeLin)  {
                com::Tween<double, com::TweenTypeLin> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuad)  {
                com::Tween<double, com::TweenTypeQuad> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeCub)  {
                com::Tween<double, com::TweenTypeCub> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuart)  {
                com::Tween<double, com::TweenTypeQuart> tween(from, to, duration);
                _perform(tween, duration, context);
            }
            else if (curveType == CurveTypeQuint)  {
                com::Tween<double, com::TweenTypeQuint> tween(from, to, duration);
                _perform(tween, duration, context);
            }   
            else if (curveType == CurveTypeExp)  {
                com::Tween<double, com::TweenTypeExp> tween(from, to, duration);
                _perform(tween, duration, context);
            } else {
                FM_THROW(Exception, "unsupported curve type: " + curveType); 
            }                                                       
        }

    }
}