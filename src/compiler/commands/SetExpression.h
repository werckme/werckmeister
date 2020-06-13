#ifndef SETEXPRESSION_HPP
#define SETEXPRESSION_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <map>
#include <fm/units.hpp>

namespace sheet {
    namespace compiler {
        class SetExpression : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetExpression.Value, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
            fm::Expression static getExpressionForString(const fm::String &expressionStr);
        private:
            typedef std::map<fm::String, fm::Expression> ExpressionMap;
            static const ExpressionMap _ExpressionMap;
        };
    }
}

#endif