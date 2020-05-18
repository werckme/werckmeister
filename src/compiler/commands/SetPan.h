#ifndef SHEET_SETPAN_HPP
#define SHEET_SETPAN_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include "ACanSpecifyInstrument.h"

namespace sheet {
    namespace compiler {
        class SetPan : public ACommand, public ACanSpecifyInstrument
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVolume.SetTo, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif