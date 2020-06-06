#ifndef ADDMOD_HPP
#define ADDMOD_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace sheet {
    namespace compiler {
        class AddMod : public ACommand
        {
        public:
            typedef ACommand Base;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.AddMod.Use, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
            virtual void setArguments(const Arguments &args) override;
            AModificationPtr theModification;
        };
    }
}

#endif