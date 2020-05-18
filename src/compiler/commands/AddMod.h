#ifndef ADDMOD_HPP
#define ADDMOD_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class AddMod : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                //FM_PARAMETER_DEF		    (argumentNames.XYZ, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif