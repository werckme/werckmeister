#ifndef SHEET_DEFINEMIDIINSTRUMENT_HPP
#define SHEET_DEFINEMIDIINSTRUMENT_HPP

#include "AMidiContextCommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class DefineMidiInstrument : public AMidiContextCommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.WithName, 	0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.OnDevice, 	1),
                FM_PARAMETER_DEF	        (argumentNames.InstrumentDef.Ch, 	    2),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.InstrumentDef.Cc, 	    3,  0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.Pc, 	    4),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContext*);
        };
    }
}

#endif