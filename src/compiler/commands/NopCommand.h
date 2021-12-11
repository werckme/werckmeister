#ifndef NOP_COMMAND_HPP
#define NOP_COMMAND_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        class NopCommand : public ACommand
        {
        public:
            com::IHasParameter::ParametersByNames parameters = {};
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr) {}
        };
    }
}

#endif