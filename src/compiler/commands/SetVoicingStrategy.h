#ifndef SETVOICINGSTRATEGY_HPP
#define SETVOICINGSTRATEGY_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class SetVoicingStrategy : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVoicingStrategy.Use, 	    0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void setArguments(const Arguments &args) override { _arguments = args; }
            virtual void execute(AContext*);
        protected:
            Arguments _arguments;
        };
    }
}

#endif