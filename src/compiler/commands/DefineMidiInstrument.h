#ifndef SHEET_DEFINEMIDIINSTRUMENT_HPP
#define SHEET_DEFINEMIDIINSTRUMENT_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class DefineMidiInstrument : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.WithName, 	0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.OnDevice, 	1),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.InstrumentDef.Cc, 	    2,  0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.Pc, 	    3),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif