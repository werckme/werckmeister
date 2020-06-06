#ifndef ADDVORSCHLAG_HPP
#define ADDVORSCHLAG_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include "AUsingAnEvent.h"

namespace sheet {
    namespace compiler {
        class AddVorschlag : public ACommand, public AUsingAnEvent
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {};
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif