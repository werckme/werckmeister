#include "SetExpression.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {

        const SetExpression::ExpressionMap SetExpression::_ExpressionMap = {
                { FM_STRING("ppppp"), com::expression::PPPPP},
				{ FM_STRING("pppp"), com::expression::PPPP },
				{ FM_STRING("ppp"), com::expression::PPP },
				{ FM_STRING("pp"), com::expression::PP },
				{ FM_STRING("p"), com::expression::P },
				{ FM_STRING("f"), com::expression::F },
				{ FM_STRING("ff"), com::expression::FF },
				{ FM_STRING("fff"), com::expression::FFF },
				{ FM_STRING("ffff"), com::expression::FFFF },
				{ FM_STRING("fffff"), com::expression::FFFFF }
        };

        com::Expression SetExpression::getExpressionForString(const com::String &expressionStr, com::Expression returnIfNotFound)
        {
            auto it = _ExpressionMap.find(expressionStr);
            if (it == _ExpressionMap.end()) {
                return returnIfNotFound;
            }
            return it->second;
        }

        void SetExpression::execute(IContextPtr  context)
        {
            auto value         = parameters[argumentNames.SetExpression.Value].value<com::String>();
            context->setExpression(getExpressionForString(value));
        }
    }
}