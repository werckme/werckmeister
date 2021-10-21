#ifndef NOP_COMMAND_HPP
#define NOP_COMMAND_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class NopCommand : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {};
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr) {}
        };
    }
}

#endif