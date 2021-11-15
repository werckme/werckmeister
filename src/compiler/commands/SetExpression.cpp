#include "SetExpression.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {

        const SetExpression::ExpressionMap SetExpression::_ExpressionMap = {
                { FM_STRING("ppppp"), fm::expression::PPPPP},
				{ FM_STRING("pppp"), fm::expression::PPPP },
				{ FM_STRING("ppp"), fm::expression::PPP },
				{ FM_STRING("pp"), fm::expression::PP },
				{ FM_STRING("p"), fm::expression::P },
				{ FM_STRING("f"), fm::expression::F },
				{ FM_STRING("ff"), fm::expression::FF },
				{ FM_STRING("fff"), fm::expression::FFF },
				{ FM_STRING("ffff"), fm::expression::FFFF },
				{ FM_STRING("fffff"), fm::expression::FFFFF }
        };

        fm::Expression SetExpression::getExpressionForString(const fm::String &expressionStr, fm::Expression returnIfNotFound)
        {
            auto it = _ExpressionMap.find(expressionStr);
            if (it == _ExpressionMap.end()) {
                return returnIfNotFound;
            }
            return it->second;
        }

        void SetExpression::execute(IContextPtr  context)
        {
            auto value         = parameters[argumentNames.SetExpression.Value].value<fm::String>();
            context->setExpression(getExpressionForString(value));
        }
    }
}