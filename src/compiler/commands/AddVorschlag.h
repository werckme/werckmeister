#ifndef ADDVORSCHLAG_HPP
#define ADDVORSCHLAG_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include "AUsingAnEvent.h"

namespace documentModel {
    namespace compiler {
        class AddVorschlag : public ACommand, public AUsingAnEvent
        {
        public:
            com::IHasParameter::ParametersByNames parameters = {};
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif